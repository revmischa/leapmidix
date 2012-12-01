//
//  main.cpp
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <time.h>
#include "MIDIXDevice.h"
#include "Visualizer.h"
#include <mach/mach.h>
#include <mach/mach_time.h>

using namespace LeapMIDIX;

void sendNote();

int main(int argc, const char * argv[]) {    
    std::cout << "Leap MIDI device initalized" << std::endl;
    
    // create virtual midi source
    Device *device = new Device();
//    device->init();
    
    // initialize visualizer
    Visualizer *viz = new Visualizer();
    viz->init();
    
    std::cin.get();
    
    delete device;
    return 0;
}

void sendNote() {
    OSStatus result;
    const unsigned kMySize = 32;
    Byte list_buffer[kMySize];
    MIDIPacketList *packetList = (MIDIPacketList*)list_buffer;
    
    Byte noteOn[3] = { 0x90, 40, 64 };
    Byte noteOff[3] = { 0x90, 40, 0 };
    
    MIDIPacket *curPacket = MIDIPacketListInit(packetList);
    
    Byte note = rand() % 40 + 25;
    noteOn[1] = note;
    noteOff[1] = note;
    curPacket = MIDIPacketListAdd(packetList, kMySize, curPacket, 0, 3, noteOn);
    curPacket = MIDIPacketListAdd(packetList, kMySize, curPacket, mach_absolute_time() + 1000, 3, noteOff);
    if (! curPacket) {
        std::cerr << "Failed to add MIDI packet" << std::endl;
        return;
    }
//    device->send(packetList);
}
