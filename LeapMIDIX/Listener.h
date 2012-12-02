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

namespace LeapMIDIX {
    class Listener : public LeapMIDI::Listener {
    public:
        void init();
        virtual ~Listener();
        
        // callback for each leap device frame
        virtual void onFrame(const Leap::Controller&);
        
        // run forever, drawing frames
        void drawLoop();
        
    protected:
        Device *device;
        Visualizer *viz;
    };
}

#endif /* defined(__LeapMIDIX__Listener__) */
