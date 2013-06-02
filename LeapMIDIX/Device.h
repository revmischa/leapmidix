//
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

// LeapMIDIX::Device represents a virtual MIDI source device
// This emits MIDI control messages

#ifndef __LeapMIDIX__LeapMIDIXDevice__
#define __LeapMIDIX__LeapMIDIXDevice__

#include <iostream>
#include <queue>
#include <pthread.h>
#include <sys/time.h>
#include <CoreMIDI/CoreMIDI.h>
#include "LeapMIDI.h"

namespace leapmidi {
    
typedef struct {
    leapmidi::midi_control_index control_index;
    leapmidi::midi_control_value control_value;

    leapmidi::midi_note_index note_index;
    leapmidi::midi_note_value note_value;
    
    int type;
    timeval timestamp;
} midi_message;

class Device {
public:
    Device();
    virtual ~Device();
    virtual void init();
    
    // thread-safe interface
    virtual void addControlMessage(leapmidi::midi_control_index controlIndex, leapmidi::midi_control_value controlValue);
    virtual void addNoteMessage(leapmidi::midi_note_index noteIndex, leapmidi::midi_note_value noteValue);
    
    // add MIDI control messages to the MIDI packet queue to transmit
    // these may block and not be safe to call from another thread
    virtual void queueMessages(std::queue<midi_message> &messages);
    virtual void queueControlPacket(leapmidi::midi_control_index control, leapmidi::midi_control_value value);
    virtual void queueNotePacket(leapmidi::midi_note_index note, leapmidi::midi_note_value value);
    
protected:
    virtual void initPacketList();
    virtual void createDevice();
    
    // thread-safe MIDI message queue
    virtual void *messageSendingThreadEntry();
    std::queue<midi_message> midiMessageQueue;
    pthread_mutex_t messageQueueMutex;
    pthread_t messageQueueThread;
    pthread_cond_t messageQueueCond;
    
    // send midi packets
    virtual OSStatus sendMIDIQueue();
    
    MIDIClientRef deviceClient;
    MIDIEndpointRef deviceEndpoint;
    MIDIPacketList *midiPacketList;
    unsigned int packetListSize;
    MIDIPacket *curPacket;
    
    std::vector<int> activeNotes;
    
private:
    static void *_messageSendingThreadEntry(void * This) {((Device *)This)->messageSendingThreadEntry(); return NULL;}

};
    
} // namespace leapmidix

#endif /* defined(__LeapMIDIX__LeapMIDIXDevice__) */
