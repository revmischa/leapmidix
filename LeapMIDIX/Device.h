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
#include "MIDITypes.h"

namespace leapmidi {
    
typedef struct {
    leapmidi::midi_control_index control_index;
    leapmidi::midi_control_value control_value;
    timeval timestamp;
} midi_message;

class Device {
public:
    Device();
    virtual ~Device();
    virtual void init();
    
    virtual void addControlMessage(leapmidi::midi_control_index controlIndex, leapmidi::midi_control_value controlValue);
            
protected:
    virtual void initPacketList();
    virtual void createDevice();
    
    // thread-safe MIDI message queue
    virtual void *messageSendingThreadEntry();
    std::queue<midi_message> midiMessageQueue;
    pthread_mutex_t messageQueueMutex;
    pthread_t messageQueueThread;
    pthread_cond_t messageQueueCond;
    
    // add MIDI control messages to the MIDI packet queue to transmit
    virtual void queueControlMessages(std::queue<midi_message> &messages);
    virtual void queueControlPacket(leapmidi::midi_control_index control, leapmidi::midi_control_value value);
    
    // send midi packets
    virtual OSStatus sendMIDIQueue();
    
    MIDIClientRef deviceClient;
    MIDIEndpointRef deviceEndpoint;
    MIDIPacketList *midiPacketList;
    unsigned int packetListSize;
    MIDIPacket *curPacket;
    
private:
    static void *_messageSendingThreadEntry(void * This) {((Device *)This)->messageSendingThreadEntry(); return NULL;}

};
    
} // namespace leapmidix

#endif /* defined(__LeapMIDIX__LeapMIDIXDevice__) */
