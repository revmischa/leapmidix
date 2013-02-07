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

namespace LeapMIDIX {
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
    
    void Device::addControlMessage(LeapMIDI::midi_control_index controlIndex, LeapMIDI::midi_control_value controlValue) {
//        int res = pthread_mutex_trylock(&messageQueueMutex);
//        if (! res) {
//            // success
//            pthread_mutex_unlock(&messageQueueMutex);
//        } else {
//            lmx_dev_debug("unable to acquire lock\n");
//        }
        
        pthread_mutex_lock(&messageQueueMutex);
//        lmx_dev_debug("main locked\n");
        
        midi_message msg;
        msg.control_index = controlIndex;
        msg.control_value = controlValue;
        gettimeofday(&msg.timestamp, NULL);
        midiMessageQueue.push(msg);
        pthread_mutex_unlock(&messageQueueMutex);
//        lmx_dev_debug("main unlocked\n");
        pthread_cond_signal(&messageQueueCond);
    }

    
    /*******/
    
    Device::Device() {
        pthread_mutex_init(&debug_output_mutex, NULL);
        
        pthread_mutex_init(&messageQueueMutex, NULL);
        pthread_mutex_init(&messageQueueCondMutex, NULL);
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
        pthread_mutex_destroy(&messageQueueCondMutex);
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
            
            // CHECK IF QUEUE IS EMPTY, IF NOT DON'T DO TIMEWAIT
            // (condvar will not be broadcasted if message is added right here)
//            int lockStatus = pthread_mutex_trylock(&messageQueueMutex);
//            if (lockStatus != 0) {
//                if (lockStatus == EBUSY) {
//                    std::cout << "EBUSY\n";
//                } else {
//                    std::cerr << "invalid mutex detected\n";
//                    exit(1);
//                }
//            }
//            if (midiMessageQueue.empty()) {
//                pthread_mutex_unlock(&messageQueueMutex);
//                continue;
//            }
            
            // acquire mutex once there is a message waiting for us
            pthread_mutex_lock(&messageQueueCondMutex);
            gettimeofday(&tv, NULL);
            ts.tv_sec = tv.tv_sec + 2; // timeout 2s
            ts.tv_nsec = 0;
            int res = pthread_cond_timedwait(&messageQueueCond, &messageQueueCondMutex, &ts);
            if (res == ETIMEDOUT) {
                // no message was waiting
                // "Upon successful return, the mutex shall have been locked and shall be owned by the calling thread"
                // is this "successful return"? do we need to unlock? i do not know
                pthread_mutex_unlock(&messageQueueCondMutex);
                //lmx_dev_debug("ETIMEDOUT\n");
                continue;
            }
            if (res != 0) {
                std::cerr << "unexpected pthread_cond_timedwait retval=" << res << std::endl;
                exit(1);
                continue;
            }
            
            if (midiMessageQueue.empty()) {
                pthread_mutex_unlock(&messageQueueCondMutex);
                continue;
            }
            
            // copy messages from shared queue into thread-local copy
            // (is there a cleaner way to do this?)
            pthread_mutex_lock(&messageQueueMutex);
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
//            lmx_dev_debug("copied queue, unlocking\n");
            
            // add control messages to MIDI packet queue
            queueControlMessages(queueCopy);
            
            // flush MIDI queue to output
            sendMIDIQueue();
            
            pthread_mutex_unlock(&messageQueueCondMutex);
        }
        
        return NULL;
    }
    
    void Device::queueControlMessages(std::queue<midi_message> &messages) {
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
            
            // we have data to send. i think this blocks
            queueControlPacket(msg.control_index, msg.control_value);
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
    void Device::queueControlPacket(LeapMIDI::midi_control_index control, LeapMIDI::midi_control_value value) {
        assert(control < 120);
        assert(value <= 127);
        
        // assign channel value
        // (hardcoded to channel 0 - Registered Parameter LSB)
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
        
        // add packet to packet list
        curPacket = MIDIPacketListAdd(midiPacketList, packetListSize, curPacket, 0, 3, packetOut);
        if (! curPacket) {
            std::cerr << "Buffer overrun on midi packet list\n";
            exit(1);
        }
    }
}

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

