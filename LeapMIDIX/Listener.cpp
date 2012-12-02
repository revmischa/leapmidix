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
    void Listener::init() {
        std::cout << "Leap MIDI device initalized" << std::endl;
        
        // create virtual midi source
        device = new Device();
        //    device->init();
        
        // initialize visualizer
        viz = new Visualizer();
        viz->init();
    }
    
    Listener::~Listener() {
        delete device;
        delete viz;
    }
    
    void Listener::onFrame(const Leap::Controller& controller) {
        // this is where the magic happens
        
        // get most recent frame
        Leap::Frame frame = controller.frame();
        
        LeapMIDI::MIDIToolController::instance().process_frame(frame);
        
//        LeapMIDI::MIDIToolController::instance().
        
        //std::map<int, LeapMIDI::MIDIToolPtr> m;
        //m = LeapMIDI::MIDIToolController::instance().tool_map_;

        /*
        for (std::map<LeapMIDI::MIDITool::ToolDescription, LeapMIDI::MIDIToolPtr>::const_iterator it = LeapMIDI::MIDIToolController::instance().tool_map_.begin();
             it != LeapMIDI::MIDIToolController::instance().tool_map_.end(); ++it) {
            
            device->write(it->second.channel(), it->second.value());
        }
         */
        
        
        // write all the values to the midi divice
        for (std::map<LeapMIDI::MIDITool::ToolDescription, LeapMIDI::MIDIToolPtr>::const_iterator it = LeapMIDI::MIDIToolController::instance().tools().begin();
             it != LeapMIDI::MIDIToolController::instance().tools().end(); ++it) {
            
            device->write(it->second->channel(), it->second->value());
        }
        
        //MIDIToolController::instance().process_frame(frame);
    }
}