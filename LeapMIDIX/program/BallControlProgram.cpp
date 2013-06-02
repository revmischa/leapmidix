//
//  SphereControlProgram.cpp
//  LeapMIDIX
//
//  Created by Snooz on 6/2/13.
//  Copyright (c) 2013 DBA int80. All rights reserved.
//

#include "BallControlProgram.h"

namespace leapmidi {

void BallControl::initGestures(std::vector<GesturePtr>&gestureRecognizers) {
    // instantiate gesture recognizer singletons
    BallGesturePtr ballGesture = make_shared<BallGesture>();
    gestureRecognizers.push_back((GesturePtr)ballGesture);
}

}