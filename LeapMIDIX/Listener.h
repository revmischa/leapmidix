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
#include "MIDIProgramControl.h"

//#define LMX_VISUALIZER_ENABLED 1

namespace LeapMIDIX {
    class Visualizer;
    
    class Listener : public LeapMIDI::Listener {
    public:
        Listener();
        virtual ~Listener();
        
        void init(Leap::Controller *controller);
        
        // run forever, drawing frames
        void drawLoop();
        
        virtual void onGestureRecognized(const Leap::Controller &controller, GestureBasePtr gesture);
        virtual void onControlUpdated(const Leap::Controller &controller, GestureBasePtr gesture, ControlBasePtr control);
        virtual void onFrame(const Leap::Controller &controller);
        
    protected:
        void processFrameRaw(const Leap::Frame &frame);
        void processFrameTools(const Leap::Frame &frame);
        
        Device *device;
        Visualizer *viz;
    };
}

#endif /* defined(__LeapMIDIX__Listener__) */
