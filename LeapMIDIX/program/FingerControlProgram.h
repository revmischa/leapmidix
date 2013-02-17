//
//  leapmidi
//
//  Created by Mischa Spiegelmock on 1/5/13.
//  Copyright (c) 2013 int80. All rights reserved.
//

#ifndef leapmidi_MIDIProgramFingerControl_h
#define leapmidi_MIDIProgramFingerControl_h

#include "LeapMIDI.h"

namespace leapmidi {
    
class FingerControl : public Program {
public:
    virtual void initGestures();
};
    
typedef shared_ptr<FingerControl> FingerControlPtr;

}

#endif
