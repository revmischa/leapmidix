//
//  Listener.h
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#ifndef __LeapMIDIX__Listener__
#define __LeapMIDIX__Listener__

#include <iostream>
#include "MIDITool.h"
#include "LeapMIDI.h"
#include "Visualizer.h"
#include "Device.h"

#define LMX_VISUALIZER_ENABLED 1

namespace LeapMIDIX {
    class Visualizer;
    
    class Listener : public LeapMIDI::Listener {
    public:
        Listener();
        virtual ~Listener();
        
        void init();
        
        // callback for each leap device frame
        void onFrame(const Leap::Controller&);
        // leap device status callbacks
        void onConnect(const Leap::Controller& controller);
        virtual void onDisconnect(const Leap::Controller& controller);
        void onInit(const Leap::Controller& controller);
        
        // run forever, drawing frames
        void drawLoop();
        
        short isLeapDeviceConnected();
        short isLeapDeviceInitialized();
        
    protected:
        short leapDeviceInitialized;
        short leapDeviceConnected;
        Device *device;
        Visualizer *viz;
    };
}

#endif /* defined(__LeapMIDIX__Listener__) */
