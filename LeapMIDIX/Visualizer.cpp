//
//  Visualizer.cpp
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#include "Visualizer.h"
#include "glew.h"
#include "glfw.h"

namespace LeapMIDIX {
    Visualizer::~Visualizer() {
        this->terminate();
    }
    
    int Visualizer::init() {        
        // main glfw turn on
        if(! glfwInit()) {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            return -1;
        }
        
        //	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
        //	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
        //	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if(! glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ))
        {
            fprintf(stderr, "Failed to open GLFW window\n");
            glfwTerminate();
            return 0;
        }
        
        // Initialize GLEW
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            return 0;
        }
        
        glfwSetWindowTitle("LeapMIDIX");
        glfwSwapBuffers();
        
        return 0;
    }
    
    void Visualizer::drawLoop() {
        do {
            glClear( GL_COLOR_BUFFER_BIT );
            
            // Swap buffers
            glfwSwapBuffers();
        } while(glfwGetWindowParam( GLFW_OPENED ));
        // run forever until window is closed
    }
    
    void Visualizer::terminate() {
        glfwTerminate();
    }
}