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

#include "Listener.h"
#include "Leap.h"

namespace LeapMIDIX {

    class Listener;

    class VerticalBar {
    public:
        VerticalBar(int originX, int originY, unsigned char initialMIDIValue,
            int initialLeapValue);
        
        void draw();
        
        void setCurrentMidiValue(unsigned char v) { currentMIDIValue_ = v; }
        void SetCurrentLeapValue(int v) { currentLeapValue_ = v; }
        
    private:
        int originX_;
        int originY_;
        
        int absoluteMin_;
        int absoluteMax_;
        
        int userDefinedMin_;
        int userDefinedMax_;
        
        int currentLeapValue_;
        unsigned char currentMIDIValue_;
    };
    
    typedef std::shared_ptr<VerticalBar> VerticalBarPtr;
    
    class Visualizer {
    public:
        ~Visualizer();
        
        // open window, set up glfw
        // returns true on success
        int init(Listener *listener_, Leap::Controller *controller_);
        
        // clean up GL
        void terminate();
        
        // run forever, drawing frames
        void drawLoop();
        
//        void drawTools(const std::map<LeapMIDI::MIDITool::ToolDescription, LeapMIDI::MIDIToolPtr>&);
        
    private:
//        std::map<LeapMIDI::MIDIToolPtr, VerticalBarPtr> toolBarMap_;

        LeapMIDIX::Listener *listener;
        Leap::Controller *controller;
    };
}

#endif /* defined(__LeapMIDIX__Visualizer__) */
