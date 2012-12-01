//
//  LeapMIDIXDevice.cpp
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#include "MIDIXDevice.h"
//#include "LeapMIDI.h"

static void fatal(const char *msg);

namespace LeapMIDIX {

Device::Device() {
    deviceClient = NULL;
    deviceEndpoint = NULL;
    deviceOutputPort = NULL;
}

void Device::init() {
    createDevice();
}

Device::~Device() {
    if (deviceOutputPort)
        MIDIPortDispose(deviceOutputPort);
    if (deviceEndpoint)
        MIDIEndpointDispose(deviceEndpoint);
    if (deviceClient)
        MIDIDeviceDispose(deviceClient);
    
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
    return MIDIReceived(deviceEndpoint, pktlist);
}

}
    
///

static void fatal(const char *msg) {
    fprintf(stderr, "Fatal error: %s\n", msg);
    exit(1);
}

