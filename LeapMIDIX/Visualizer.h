//
//  Visualizer.h
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#ifndef __LeapMIDIX__Visualizer__
#define __LeapMIDIX__Visualizer__

#include <iostream>
#include <map>

#include "MIDITool.h"
#include "Listener.h"

namespace LeapMIDIX {

    class Listener;

    class VerticalBar {
    public:
        VerticalBar(int origin_x, int origin_y);
        
        void draw();
        
    private:
        int absolute_min_;
        int absolute_max_;
        
        int user_set_min_;
        int user_set_max_;
        
        int current_value_;
    };
    
    class Visualizer {
    public:
        ~Visualizer();
        
        // open window, set up glfw
        // returns true on success
        int init(Listener *listener);
        
        // clean up GL
        void terminate();
        
        // run forever, drawing frames
        void drawLoop();
        
        // TODO: fill this in
        void drawTools(const std::map<LeapMIDI::MIDITool::ToolDescription, LeapMIDI::MIDIToolPtr>&) { }
        
    private:

        LeapMIDIX::Listener *listener;        
    };
}

#endif /* defined(__LeapMIDIX__Visualizer__) */
