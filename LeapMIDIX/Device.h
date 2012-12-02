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
#include <CoreMIDI/CoreMIDI.h>
#include "LeapMIDI.h"

namespace LeapMIDIX {
    class Device {
    public:
        Device();
        ~Device();
        void init();
                
        // send a control message
        void writeControl(unsigned char control, unsigned char value);
        
        // send midi packets
        OSStatus send(const MIDIPacketList *pktlist);
        
    protected:
        void initPacketList();
        void createDevice();
        MIDIClientRef deviceClient;
        MIDIEndpointRef deviceEndpoint;
        MIDIPacketList *midiPacketList;
        unsigned int packetListSize;
        MIDIPacket *curPacket;
    };
}

#endif /* defined(__LeapMIDIX__LeapMIDIXDevice__) */
