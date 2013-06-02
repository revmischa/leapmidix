//
//  leapmidi
//
//  Created by Chris Czub on 2/16/13.
//  Copyright (c) 2013 int80. All rights reserved.
//

#ifndef leapmidi_MIDIProgramFingerNote_h
#define leapmidi_MIDIProgramFingerNote_h

#include "LeapMIDI.h"
#include "MIDIProgram.h"
#include "FingerGesture.h"

using namespace std;

namespace leapmidi {

class FingerNote : public MIDIProgram {
public:
    virtual void initGestures(std::vector<GesturePtr>&gestureRecognizers);
};

typedef shared_ptr<FingerNote> FingerNotePtr;
    
}

#endif
