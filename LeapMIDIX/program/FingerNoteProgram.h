//
//  leapmidi
//
//  Created by Chris Czub on 2/16/13.
//  Copyright (c) 2013 int80. All rights reserved.
//

#ifndef leapmidi_MIDIProgramFingerNote_h
#define leapmidi_MIDIProgramFingerNote_h

#include "LeapMIDI.h"

namespace leapmidi {
    
    class FingerNote : public NoteProgram {
    public:
        virtual void initGestures();
    };
    
    typedef shared_ptr<FingerNote> FingerNotePtr;
    
}

#endif
