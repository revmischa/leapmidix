//
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#include "Device.h"
#include <CoreMIDI/CoreMIDI.h>
#include <CoreMIDI/MIDIServices.h>

pthread_mutex_t debug_output_mutex;
static void fatal(const char *msg);
static void lmx_dev_debug(const char *msg);

namespace leapmidi {
    
void Device::init() {
    initPacketList();
    createDevice();
    
    // start message sending queue
    int res = pthread_create(&messageQueueThread, NULL, _messageSendingThreadEntry, this);
    if (res) {
        std::cerr << "pthread_create failed " << res << std::endl;
        exit(1);
    }
}

void Device::addControlMessage(leapmidi::midi_control_index controlIndex, leapmidi::midi_control_value controlValue) {
    pthread_mutex_lock(&messageQueueMutex);
    
    midi_message msg;
    msg.control_index = controlIndex;
    msg.control_value = controlValue;
    msg.type = MSG_CONTROL;
    gettimeofday(&msg.timestamp, NULL);
    midiMessageQueue.push(msg);
    pthread_mutex_unlock(&messageQueueMutex);
    pthread_cond_signal(&messageQueueCond);
}
    
    void Device::addNoteMessage(leapmidi::midi_note_index noteIndex, leapmidi::midi_note_value noteValue) {
        pthread_mutex_lock(&messageQueueMutex);
        
        midi_message msg;
        msg.note_index = noteIndex;
        msg.note_value = noteValue;
        msg.type = MSG_NOTE;
        gettimeofday(&msg.timestamp, NULL);
        midiMessageQueue.push(msg);
        pthread_mutex_unlock(&messageQueueMutex);
        pthread_cond_signal(&messageQueueCond);
    }


/*******/

Device::Device() {
    pthread_mutex_init(&debug_output_mutex, NULL);
    
    pthread_mutex_init(&messageQueueMutex, NULL);
    pthread_cond_init(&messageQueueCond, NULL);
    
    packetListSize = 512; // buffer size for midi packet messages
    deviceClient = NULL;
    deviceEndpoint = NULL;
    midiPacketList = NULL;
}

Device::~Device() {
    if (deviceEndpoint)
        MIDIEndpointDispose(deviceEndpoint);
    if (deviceClient)
        MIDIDeviceDispose(deviceClient);
    if (midiPacketList)
        free(midiPacketList);
    
    if (messageQueueThread)
        pthread_cancel(messageQueueThread);
    
    pthread_mutex_destroy(&messageQueueMutex);
    pthread_cond_destroy(&messageQueueCond);
    
    std::cout << "closed down device\n";
}

void Device::initPacketList() {
    if (midiPacketList) {
        free(midiPacketList);
        midiPacketList = NULL;
    }
    
    midiPacketList = (MIDIPacketList *)malloc(packetListSize * sizeof(char));
    curPacket = MIDIPacketListInit(midiPacketList);
}

void Device::createDevice() {
    OSStatus result;
    
    result = MIDIClientCreate(CFSTR("LeapMIDIX"), NULL, NULL, &deviceClient);
    if (result)
        fatal("Failed to create MIDI client");
    
    result = MIDISourceCreate(deviceClient, CFSTR("LeapMIDIX Control"), &deviceEndpoint);
    if (result)
        fatal("Failed to create MIDI source");
    
}

void *Device::messageSendingThreadEntry() {
//        std::cout << "messageSendingThreadEntry\n";
    struct timeval tv;
    struct timespec ts;

    while (1) {
        pthread_testcancel();
        
        // wait for next item to go in the queue
        pthread_mutex_lock(&messageQueueMutex);
        
        // check if we have anything in the queue to send
        if (midiMessageQueue.empty()) {
            // wait on next item
            gettimeofday(&tv, NULL);
            ts.tv_sec = tv.tv_sec + 2; // timeout 2s
            ts.tv_nsec = 0;
            // wait until other thread posts a notif
            int res = pthread_cond_timedwait(&messageQueueCond, &messageQueueMutex, &ts);
            if (res == ETIMEDOUT) {
                // no message was waiting
//                lmx_dev_debug("ETIMEDOUT\n");
                pthread_mutex_unlock(&messageQueueMutex);
                continue;
            }
            if (res != 0) {
                std::cerr << "unexpected pthread_cond_timedwait retval=" << res << std::endl;
                exit(1);
                continue;
            }
            
            if (midiMessageQueue.empty()) {
                lmx_dev_debug("EMPTY\n");
                pthread_mutex_unlock(&messageQueueMutex);
                continue;
            }
        }
        
        // copy messages from shared queue into thread-local copy
        // (is there a cleaner way to do this?)
//            lmx_dev_debug("queue copy lock acquired\n");
        std::queue<midi_message> queueCopy;
        while (! midiMessageQueue.empty()) {
            midi_message msg = midiMessageQueue.front();
            queueCopy.push(msg);
            midiMessageQueue.pop();
        }
        // unlock
        if (pthread_mutex_unlock(&messageQueueMutex) != 0) {
            std::cerr << "message queue mutex unlock failure\n";
            exit(1);
        }
        
        // add control messages to MIDI packet queue
        queueMessages(queueCopy);
        
        // flush MIDI queue to output
        sendMIDIQueue();
    }
    
    return NULL;
}

void Device::queueMessages(std::queue<midi_message> &messages) {
    struct timeval tv;

    while (! messages.empty()) {
        midi_message msg = messages.front();
        messages.pop();
        
        // figure out when this packet was added
        gettimeofday(&tv, NULL);
        double elapsedTime = (tv.tv_sec - msg.timestamp.tv_sec) * 1000.0;      // sec to ms
        elapsedTime += (tv.tv_usec - msg.timestamp.tv_usec) / 1000.0;   // us to ms
        if (elapsedTime > 2) {
            // message was triggered too long ago, we don't want to emit old messages
            std::cerr << "Warning, MIDI control message latency of " << elapsedTime << "ms detected.\n";
            continue; // drop message
        }
        
        if (msg.type == MSG_CONTROL) {
            // we have data to send. i think this blocks
            queueControlPacket(msg.control_index, msg.control_value);
        } else if (msg.type == MSG_NOTE) {
            // we have data to send. i think this blocks
            queueNotePacket(msg.note_index, msg.note_value);
        } else {
            printf("Unknown MIDI message type; ignoring\n");
        }
    }
}

// "send" a packet, really pretends that our virtual device source received a packet
OSStatus Device::sendMIDIQueue() {
    // send current packet list
    OSStatus res = MIDIReceived(deviceEndpoint, midiPacketList);
    
    // reinitialize packet list, MIDIReceived does not appear to flush the list
    // none of this is really documented but this seems to work ok
    initPacketList();
    
    return res;
}

// control = MIDI control #, 0-119
// value = MIDI control message value, 0-127
void Device::queueControlPacket(leapmidi::midi_control_index control, leapmidi::midi_control_value value) {
    assert(control < 120);
    assert(value <= 127);
    
    // assign channel value
    // (hardcoded to channel 0 for now)
    unsigned char channelBase = 0xB0;
    unsigned char channel = 0;
    unsigned char midiChannel = channelBase + channel;
    
    // assign control selector
    unsigned char controlBase = 0x00;
    unsigned char midiControl = controlBase + control;
    
    // build midi packet
    Byte packetOut[3];
    packetOut[0] = midiChannel;
    packetOut[1] = midiControl;
    packetOut[2] = value;
    
    int z;
    printf("Sending MIDI packet: ");
    for (z = 0; z < 3; z++)
    {
        if (z > 0) printf(":");
        printf("%02X", packetOut[z]);
    }
    printf("\n");

    
    // add packet to packet list
    curPacket = MIDIPacketListAdd(midiPacketList, packetListSize, curPacket, 0, 3, packetOut);
    if (! curPacket) {
        std::cerr << "Buffer overrun on midi packet list\n";
        exit(1);
    }
}
    // note = MIDI note #, 0-119
    // value = MIDI note message velocity, 0-127
    void Device::queueNotePacket(leapmidi::midi_note_index note, leapmidi::midi_note_value value) {
        assert(note < 127);
        assert(value <= 127);
        
        // assign channel value
        // (hardcoded to channel 0 - Registered Parameter LSB)
        unsigned char channelBase = 0x90;
        unsigned char channel = 0;
        
        if (value < 50) {
            // if value is less than 50, turn note off
            channelBase = 0x80;
        }

        unsigned char midiChannel = channelBase + channel;
        
        // assign control selector
        unsigned char noteBase = 0x48;
        unsigned char midiNote = noteBase + note;
        
        if (value >= 50) {
            //unsigned char midiControl = noteBase + note;
            int i = 0;
            for (i = 0; i < activeNotes.size(); i++) {
                if (activeNotes.at(i) == midiNote) {
                    // this note is already on, don't try playing it again
                    printf("Not playing another note on\n");
                    return;
                }
            }
            
            activeNotes.push_back(midiNote);
        } else if (value < 50) {
            //unsigned char midiControl = noteBase + note;
            int i = 0;
            for (i = 0; i < activeNotes.size(); i++) {
                if (activeNotes.at(i) == midiNote) {
                    // remove from list of active notes
                    activeNotes.erase(activeNotes.begin() + i);
                }
            }
        }
        
        // build midi packet
        Byte packetOut[3];
        packetOut[0] = midiChannel;
        packetOut[1] = midiNote;
        packetOut[2] = 0x7F;
        //        packetOut[2] = value;
        
        int z;
        printf("Sending MIDI packet: ");
        for (z = 0; z < 3; z++)
        {
            if (z > 0) printf(":");
            printf("%02X", packetOut[z]);
        }
        printf("\n");
        
        
        // add packet to packet list
        curPacket = MIDIPacketListAdd(midiPacketList, packetListSize, curPacket, 0, 3, packetOut);
        if (! curPacket) {
            std::cerr << "Buffer overrun on midi packet list\n";
            exit(1);
        }
    }


} // namespace leapmidi

///

static void lmx_dev_debug(const char *msg) {
    pthread_mutex_lock(&debug_output_mutex);
    fprintf(stderr, "DEBUG: %s", msg);
    pthread_mutex_unlock(&debug_output_mutex);
}

static void fatal(const char *msg) {
    fprintf(stderr, "Fatal error: %s\n", msg);
    exit(1);
}

