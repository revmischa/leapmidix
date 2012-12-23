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
    Listener listener;
    listener.init();
    Leap::Controller controller;
    controller.addListener(listener);
    
    // run forever
    try {
        listener.drawLoop();
    } catch (const std::exception &ex) {
        std::cerr << "Caught exception: " << ex.what() << std::endl;
        exit(1);
    }
    
    return 0;
}
