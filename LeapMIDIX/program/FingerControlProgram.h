//
//  leapmidi
//
//  Created by Mischa Spiegelmock on 1/5/13.
//  Copyright (c) 2013 int80. All rights reserved.
//

#ifndef leapmidi_MIDIProgramFingerControl_h
#define leapmidi_MIDIProgramFingerControl_h

#include "LeapMIDI.h"
#include "MIDIProgram.h"
#include "FingerGesture.h"

using namespace std;

namespace leapmidi {
    
class FingerControl : public MIDIProgram {
public:
    virtual void initGestures(std::vector<GesturePtr>&));

    typedef shared_ptr<FingerControl> FingerControlPtr;
};
    
}

#endif
