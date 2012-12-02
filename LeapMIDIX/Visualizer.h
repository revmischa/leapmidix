//
//  Visualizer.h
//  LeapMIDIX
//
//  Created by Snooz on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#ifndef __LeapMIDIX__Visualizer__
#define __LeapMIDIX__Visualizer__

#include <iostream>
#include <map>

#include "MIDITool.h"

namespace LeapMIDIX {
    class Visualizer {
    public:
        void init();
        
        // TODO: fill this in
        void drawTools(const std::map<LeapMIDI::MIDITool::ToolDescription, LeapMIDI::MIDIToolPtr>&) { }
    
    };
}

#endif /* defined(__LeapMIDIX__Visualizer__) */
