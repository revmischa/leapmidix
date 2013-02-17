//
//  leapmidi
//
//  Created by Mischa Spiegelmock on 1/5/13.
//  Copyright (c) 2013 int80. All rights reserved.
//

// Program to send live MIDI control messages

#include <stdio.h>
#include "MIDIProgramControl.h"

const short MIDI_DEBUG = 1;

using namespace std;

namespace LeapMIDI {
    namespace Program {
        Control::Control() {
            initGestures();
        }
        
        void Control::initGestures() {
            // instantiate gesture recognizer singletons
            //        Gesture::Ball *ballGesture = new Gesture::Ball();
            //        gestureRecognizers.push_back(ballGesture);
            GestureFingerPtr fingerGesture = make_shared<Gesture::Finger>();
            GestureBallPtr ballGesture = make_shared<Gesture::Ball>();
            gestureRecognizers.push_back((GestureBasePtr)fingerGesture);
            gestureRecognizers.push_back((GestureBasePtr)ballGesture);
        }
                
        void Control::onGestureRecognized(const Leap::Controller &controller, GestureBasePtr gesture) {
        }
        
        void Control::onControlUpdated(const Leap::Controller &controller, GestureBasePtr gesture, ControlBasePtr control) {
            midi_control_value val = control->mappedValue();
            midi_control_index idx = control->controlIndex();
            
            if (MIDI_DEBUG) {
                cout << "recognized control index " << idx
                << " (" << control->description() << ")"
                << ", raw value: "
                << control->rawValue() << " mapped value: " << val << endl;
            }
        }
    }
}

