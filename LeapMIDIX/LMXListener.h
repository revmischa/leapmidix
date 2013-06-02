//
//  LMXListener.h
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#ifndef __LeapMIDIX__Listener__
#define __LeapMIDIX__Listener__

#include <iostream>
#include "Visualizer.h"
#include "Device.h"
#include "Leap.h"
#include "LeapMIDI.h"
#include "MIDIListener.h"
#include "FingerControlProgram.h"
#include "FingerNoteProgram.h"
#include "MIDINote.h"

//#define LMX_VISUALIZER_ENABLED 1

namespace leapmidi {
    
class Visualizer;

class LMXListener : public leapmidi::Listener {
public:
    LMXListener();
    virtual ~LMXListener();
    
    void init(Leap::Controller *controller);
    
    // run forever, drawing frames
    void drawLoop();
    
    virtual void onGestureRecognized(const Leap::Controller &controller, GesturePtr gesture);
    virtual void onControlUpdated(const Leap::Controller &controller, GesturePtr gesture, ControlPtr control);
    virtual void onNoteUpdated(const Leap::Controller &controller, GesturePtr gesture, NotePtr note);
    
protected:
    void processFrameRaw(const Leap::Frame &frame);
    void processFrameTools(const Leap::Frame &frame);
    
    Device *device;
    Visualizer *viz;
};
    
}

#endif /* defined(__LeapMIDIX__Listener__) */
