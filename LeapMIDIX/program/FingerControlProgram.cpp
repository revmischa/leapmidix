//
//  leapmidi
//
//  Created by Mischa Spiegelmock on 1/5/13.
//  Copyright (c) 2013 int80. All rights reserved.
//

// Program to send live MIDI control messages

#include <stdio.h>
#include "FingerControlProgram.h"

namespace leapmidi {

void FingerControl::initGestures() {
    // instantiate gesture recognizer singletons
    FingerGesturePtr fingerGesture = make_shared<FingerGesture>();
    gestureRecognizers.push_back((GesturePtr)fingerGesture);
}
    
}

