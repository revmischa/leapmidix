//
//  LeapMIDIXDevice.cpp
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#include "Device.h"
#include <CoreMIDI/CoreMIDI.h>
#include <CoreMIDI/MIDIServices.h>
//#include "LeapMIDI.h"

static void fatal(const char *msg);

namespace LeapMIDIX {

    Device::Device() {
        packetListSize = 512;
        deviceClient = NULL;
        deviceEndpoint = NULL;
        deviceOutputPort = NULL;
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
        if (deviceOutputPort)
            MIDIPortDispose(deviceOutputPort);
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
        
        result = MIDIOutputPortCreate(deviceClient, CFSTR("LeapMIDIX Output Port"), &deviceOutputPort);
        if (result)
            fatal("Failed to create MIDI output port");
        
    }

    OSStatus Device::send(const MIDIPacketList *pktlist) {
        OSStatus res = MIDIReceived(deviceEndpoint, pktlist);
//        curPacket = MIDI
        
        // probably not thread-safe, if a frame callback is called while this is happening
        initPacketList();

        return res;
    }
    
    void Device::write(unsigned char control, unsigned char value) {
//        assert(control < 120);
        assert(value <= 127);
        
        // assign channel value
        unsigned char channelBase = 0xB0;
        unsigned char channel = 0;
        unsigned char midiChannel = channelBase + channel;
        
        // assign control selector
        unsigned char controlBase = 0x64;
        unsigned char midiControl = controlBase + control;
        
        Byte packetOut[3];
        packetOut[0] = midiChannel;
        packetOut[1] = midiControl;
        packetOut[2] = value;
        
        curPacket = MIDIPacketListAdd(midiPacketList, packetListSize, curPacket, 0, 3, packetOut);
        if (! curPacket) {
            std::cerr << "Buffer overrun on midi packet list\n";
            exit(1);
        }
        
        std::cout << "Emitted channel " << (int)channel << " value: " << (int)value << std::endl;
        this->send(midiPacketList);
    }
}
    
///

static void fatal(const char *msg) {
    fprintf(stderr, "Fatal error: %s\n", msg);
    exit(1);
}

