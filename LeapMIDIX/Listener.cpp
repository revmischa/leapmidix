//
//  Listener.cpp
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#include <memory>
#include <map>

#include "Listener.h"

namespace LeapMIDIX {
    Listener::Listener() {
        viz = NULL;
        device = NULL;
    }
    
    void Listener::init(Leap::Controller *controller) {
        std::cout << "Leap MIDI device initalized" << std::endl;
        
        // create virtual midi source
        device = new Device();
        device->init();
        
        viz = NULL;
#ifdef LMX_VISUALIZER_ENABLED
        // initialize visualizer
        viz = new Visualizer();
        viz->init(this);
#endif
    }
    
    Listener::~Listener() {
        if (viz)
            delete viz;
        if (device)
            delete device;
    }
    
    void Listener::onGestureRecognized(const Leap::Controller &controller, LeapMIDI::Gesture::Base &gesture) {
        LeapMIDI::Listener::onGestureRecognized(controller, gesture);
    }
    
    void Listener::onControlUpdated(const Leap::Controller &controller, LeapMIDI::Gesture::Base &gesture, LeapMIDI::Control::Base &control) {
        // call superclass method
        LeapMIDI::Listener::onControlUpdated(controller, gesture, control);
        
        // draw gesture and control output
        // ...
        
        // control
        LeapMIDI::midi_control_index controlIndex = control.controlIndex();
        // control value
        LeapMIDI::midi_control_value val = control.mappedValue();
        
        device->addControlMessage(controlIndex, val);
    }

    void Listener::drawLoop() {
#ifdef LMX_VISUALIZER_ENABLED
        viz->drawLoop();
#else
        std::cin.get();
#endif
    }
}