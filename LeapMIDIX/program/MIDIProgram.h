//
//  MIDIProgram.h
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 1/5/13.
//  Copyright (c) 2013 int80. All rights reserved.
//

#ifndef __LeapMIDIX__MIDIProgram__
#define __LeapMIDIX__MIDIProgram__

#include "LeapMIDI.h"
#include <vector>
#include "MIDIGesture.h"

using namespace std;

namespace leapmidi {
    
    class MIDIProgram {
    public:
        virtual void initGestures(std::vector<GesturePtr>&) = 0;
    };
    
    typedef shared_ptr<MIDIProgram> MIDIProgramPtr;
    
}

#endif
