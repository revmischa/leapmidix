//
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#ifndef __LeapMIDIX__LeapMIDIXDevice__
#define __LeapMIDIX__LeapMIDIXDevice__

#include <iostream>
#include <CoreMIDI/CoreMIDI.h>
//#include <GL/glew.h>
#include <glfw.h>
#include "LeapMIDI.h"

namespace LeapMIDIX {
    class Device {
    public:
        Device();
        ~Device();
        void init();
        
        void initPacketList();
        void write(unsigned char channel, unsigned char value);
        OSStatus send(const MIDIPacketList *pktlist);
        
    protected:
        void createDevice();
        MIDIClientRef deviceClient;
        MIDIEndpointRef deviceEndpoint;
        MIDIPacketList *midiPacketList;
        unsigned int packetListSize;
        MIDIPacket *curPacket;
    };
}

#endif /* defined(__LeapMIDIX__LeapMIDIXDevice__) */
