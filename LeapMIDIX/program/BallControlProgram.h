//
//  leapmidi
//
//  Created by Mischa Spiegelmock on 1/5/13.
//  Copyright (c) 2013 int80. All rights reserved.
//

#ifndef leapmidi_MIDIProgramBallControl_h
#define leapmidi_MIDIProgramBallControl_h

#include "LeapMIDI.h"
#include "MIDIProgram.h"
#include "BallGesture.h"

using namespace std;

namespace leapmidi {
    
class BallControl : public MIDIProgram {
public:
    virtual void initGestures(std::vector<GesturePtr>&);
};

typedef shared_ptr<BallControl> BallControlProgramPtr;
    
}

#endif
