//
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#include "Device.h"
#include <CoreMIDI/CoreMIDI.h>
#include <CoreMIDI/MIDIServices.h>

static void fatal(const char *msg);

namespace LeapMIDIX {
    Device::Device() {
        packetListSize = 512; // buffer size for midi packet messages
        deviceClient = NULL;
        deviceEndpoint = NULL;
        midiPacketList = NULL;
    }

    void Device::init() {
        initPacketList();
        createDevice();
    }
    
    void Device::initPacketList() {
        if (midiPacketList) {
            free(midiPacketList);
            midiPacketList = NULL;
        }
        
        midiPacketList = (MIDIPacketList *)malloc(packetListSize * sizeof(char));
        curPacket = MIDIPacketListInit(midiPacketList);
    }

    Device::~Device() {
        if (deviceEndpoint)
            MIDIEndpointDispose(deviceEndpoint);
        if (deviceClient)
            MIDIDeviceDispose(deviceClient);
        if (midiPacketList)
            free(midiPacketList);
        
        std::cout << "closed down device\n";
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

    // "send" a packet, really pretends that our virtual device source received a packet
    OSStatus Device::send(const MIDIPacketList *pktlist) {
        // send current packet list
        OSStatus res = MIDIReceived(deviceEndpoint, pktlist);
        
        // reinitialize packet list, MIDIReceived does not appear to flush the list
        // none of this is really documented but this seems to work ok
        // FIXME: probably not thread-safe, if a frame callback is called while this is happening
        initPacketList();

        return res;
    }
    
    // control = MIDI control #, 0-119
    // value = MIDI control message value, 0-127
    void Device::writeControl(unsigned char control, unsigned char value) {
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
        
        std::cout << "Emitted channel " << (int)channel << " value: " << (int)value << std::endl;
        
        // "send" packet
        this->send(midiPacketList);
    }
}
    
///

static void fatal(const char *msg) {
    fprintf(stderr, "Fatal error: %s\n", msg);
    exit(1);
}

