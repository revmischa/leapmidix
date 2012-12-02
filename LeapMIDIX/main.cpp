//
//  main.cpp
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <time.h>
#include "Listener.h"

using namespace LeapMIDIX;

void sendNote();

int main(int argc, const char * argv[]) {
    // start listening for events
    Listener *listener = new Listener();
    listener->init();
    Leap::Controller controller(listener);
    
    // run forever
    std::cin.get();
    
    return 0;
}
