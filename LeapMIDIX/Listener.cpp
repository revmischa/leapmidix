//
//  Listener.cpp
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#include <memory>
#include <map>

#include "MIDITool.h"

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
    
    void Listener::onFrame(const Leap::Controller& controller) {
        // this is where the magic happens
        
        // get most recent frame
        Leap::Frame frame = controller.frame();
        
        /*
        if (frame.hands().empty())
            return;
        
        Leap::Hand hand = frame.hands()[0];
        if (hand.fingers().empty())
            return;
        
        Leap::Finger finger = hand.fingers()[0];
        unsigned int value = finger.tip().position.y;
        if (value > 127) value = 127; // TODO: map into range
        device->write(0, value);
        return;
        */
         
        LeapMIDI::MIDIToolController::instance().process_frame(frame);
        
        // write all the values to the midi divice
        for (std::map<LeapMIDI::MIDITool::ToolDescription, LeapMIDI::MIDIToolPtr>::const_iterator it = LeapMIDI::MIDIToolController::instance().tools().begin();
             it != LeapMIDI::MIDIToolController::instance().tools().end(); ++it) {
            
            if (it->second->active())
                device->writeControl(it->second->control(), it->second->value());
        }
        
        viz->drawTools(LeapMIDI::MIDIToolController::instance().tools());
        
        //MIDIToolController::instance().process_frame(frame);
    }
    
    void Listener::drawLoop() {
#ifdef LMX_VISUALIZER_ENABLED
        viz->drawLoop();
#else
        std::cin.get();
#endif
    }
}