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
#include "Visualizer.h"
#include "Device.h"
#include "Leap.h"
#include "LeapMIDI.h"

//#define LMX_VISUALIZER_ENABLED 1

namespace LeapMIDIX {
    class Visualizer;
    
    class Listener : public LeapMIDI::Listener {
    public:
        Listener();
        virtual ~Listener();
        
        void init();
        
        // callback for each leap device frame
//        virtual void onFrame(const Leap::Controller&);
        // leap device status callbacks
        virtual void onConnect(const Leap::Controller& controller);
        virtual void onDisconnect(const Leap::Controller& controller);
        virtual void onInit(const Leap::Controller& controller);
        
        // events from LeapMIDI
        virtual void onGestureRecognized(const Leap::Controller &controller, LeapMIDI::Gesture::Base &gesture);
        virtual void onControlUpdated(const Leap::Controller &controller, LeapMIDI::Gesture::Base &gesture, LeapMIDI::Control::Base &control);
        
        // run forever, drawing frames
        void drawLoop();
        
        short isLeapDeviceConnected();
        short isLeapDeviceInitialized();
        
    protected:
        void processFrameRaw(const Leap::Frame &frame);
        void processFrameTools(const Leap::Frame &frame);
        
        short leapDeviceInitialized;
        short leapDeviceConnected;
        Device *device;
        Visualizer *viz;
    };
}

#endif /* defined(__LeapMIDIX__Listener__) */
