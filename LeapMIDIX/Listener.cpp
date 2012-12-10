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
    
    void Listener::init() {
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
    
    void LeapMIDIX::Listener::onInit(const Leap::Controller& controller) {
        std::cout << "Leap device initialized\n";
        leapDeviceInitialized = 1;
    }
    
    void LeapMIDIX::Listener::onConnect(const Leap::Controller& controller) {
        std::cout << "Leap device connected\n";
        leapDeviceConnected = 1;
    }
    
    void LeapMIDIX::Listener::onDisconnect(const Leap::Controller& controller) {
        std::cout << "Leap device disconnected\n";
        leapDeviceConnected = 0;
    }
    short Listener::isLeapDeviceInitialized() {
        return leapDeviceInitialized;
    }
    short Listener::isLeapDeviceConnected() {
        return leapDeviceConnected;
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