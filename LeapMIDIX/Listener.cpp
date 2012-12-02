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
        
        if (frame.hands().empty())
            return;

        processFrameRaw(frame);
//        processFrameTools(frame);
    }
    
    // really simple finger->midi control processing, useful for testing and demoing
    void Listener::processFrameRaw(const Leap::Frame &frame) {        
        Leap::Hand hand = frame.hands()[0];
        unsigned int fingerCount = (unsigned int)hand.fingers().size();

        if (! fingerCount)
            return;
        
        unsigned int value = 0;
        
        // average finger Y coords
        for (int i = 0; i < fingerCount; i++) {
            value += hand.fingers()[i].tip().position.y;
        }
        value /= fingerCount;
        
        value -= 30; // min Y value
        if (value > 127) value = 127; // TODO: map into range
        device->writeControl(fingerCount, value);
    }
    
    // fancy processing of frames to generate abstract tools
    void Listener::processFrameTools(const Leap::Frame &frame) {
        LeapMIDI::MIDIToolController::instance().process_frame(frame);
        
        // write all the values to the midi divice
        for (std::map<LeapMIDIControl, LeapMIDI::MIDIToolPtr>::const_iterator it = LeapMIDI::MIDIToolController::instance().tools().begin();
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