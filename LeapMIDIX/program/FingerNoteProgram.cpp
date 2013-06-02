//
//  leapmidi
//
//  Created by Chris Czub on 2/16/13.
//  Copyright (c) 2013 int80. All rights reserved.
//

// Program to send live MIDI note messages

#include <stdio.h>
#include "FingerNoteProgram.h"

namespace leapmidi {
    
    void FingerNote::initGestures(std::vector<GesturePtr>&gestureRecognizers) {
        // instantiate gesture recognizer singletons
        FingerGesturePtr fingerGesture = make_shared<FingerGesture>();
        gestureRecognizers.push_back((GesturePtr)fingerGesture);
    }
    
}

