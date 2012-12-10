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
    VerticalBar::VerticalBar(int originX, int originY, unsigned char initialMIDIValue,
        int initialLeapValue) {
        originX_ = originX;
        originY_ = originY;
        
        absoluteMin_ = 0;
        absoluteMax_ = 500;
        
        userDefinedMin_ = 40;
        userDefinedMax_ = 300;
        
        currentMIDIValue_ = initialMIDIValue;
        currentLeapValue_ = initialLeapValue;
    }
    
    void VerticalBar::draw() {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE);
        glVertex2d(originX_, originY_);
        glVertex2d(originX_, originY_ + 50);
        glEnd();
    }
    
    Visualizer::~Visualizer() {
        this->terminate();
    }
    
    int Visualizer::init(Listener *listener_) {
        // save listener instance so we can interrogate it when drawing
        listener = listener_;
        
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

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        float aspect_ratio = ((float)1024) / 768;
        
        glFrustum(.5, -.5, -.5 * aspect_ratio, .5 * aspect_ratio, 1, 50);
        
        glMatrixMode(GL_MODELVIEW);
        
        return 0;
    }
    
    void Visualizer::drawLoop() {
        do {
            glClear( GL_COLOR_BUFFER_BIT );
            
            
            glClearColor(listener->isLeapDeviceConnected() ? 0 : 1, 0, 0, 0);
            
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            /*
            for (std::map<LeapMIDI::MIDIToolPtr, VerticalBarPtr>::iterator it =
                toolBarMap_.begin(); it != toolBarMap_.end(); ++it) {
                it->second->draw();
            }
             */
            
            //glClearColor(0.0, 0.0, 0.0, 0.0);  //Set the cleared screen colour to black
            glViewport(0, 0, 1024, 768);   //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window
            
            //Set up the orthographic projection so that coordinates (0, 0) are in the top left
            //and the minimum and maximum depth is -10 and 10. To enable depth just put in
            //glEnable(GL_DEPTH_TEST)
            
            glMatrixMode(GL_PROJECTION);
            
            glLoadIdentity();
            
            gluOrtho2D(0.0, 1024, 768.0, 0.0);
            
            //glOrtho(0, 1024, 768, 0, -10, 10);
            
            
            /*
            // reset view matrix
            glLoadIdentity();
            // move view back a bit
            glTranslatef(0, 0, -30);
            // apply the current rotation
            //glRotatef(0, 0, 1, 0);
            //glRotatef(0, 0, 0, 1);
            // by repeatedly rotating the view matrix during drawing, the
            // squares end up in a circle
            int i = 0, squares = 15;
            float green = 0.6;
            float red = 0, blue = 1;
            for (; i < squares; ++i) {
                glRotatef(360.0/squares, 0, 0, 1);
                // colors change for each square
                red += 1.0/12;
                blue -= 1.0/12;
            
                // Draws a square with a gradient color at coordinates 0, 10
                glBegin(GL_QUADS);
                {
                    glColor3f(red, green, blue);
                    glVertex2i(1, 11);
                    glColor3f(red * .8, green * .8, blue * .8);
                    glVertex2i(-1, 11);
                    glColor3f(red * .5, green * .5, blue * .5);
                    glVertex2i(-1, 9);
                    glColor3f(red * .8, green * .8, blue * .8);
                    glVertex2i(1, 9);
                }
                glEnd();
             
            }
             */
            
            glColor3f(255.0, 1.0, 1.0);
            glBegin(GL_LINE);
            glVertex2d(0, 0);
            glVertex2d(10, 10 + 50);
            glEnd();
            
            // Swap buffers
            glfwSwapBuffers();
        } while(glfwGetWindowParam(GLFW_OPENED));
        // run forever until window is closed
    }
    
    void Visualizer::terminate() {
        glfwTerminate();
    }
    
    /*
    void Visualizer::drawTools(const std::map<LeapMIDI::MIDITool::ToolDescription,
        LeapMIDI::MIDIToolPtr>& tools) {
        for (std::map<LeapMIDIControl,
             LeapMIDI::MIDIToolPtr>::const_iterator it = tools.begin(); it !=
             tools.end(); ++it) {
            
            if (!toolBarMap_[it->second]) {
                // get a new x y z value
                toolBarMap_[it->second] = VerticalBarPtr(new
                    VerticalBar(10, 10, it->second->value(), it->second->leapValue()));
            }
            
            toolBarMap_[it->second]->setCurrentMidiValue(it->second->value());
            toolBarMap_[it->second]->SetCurrentLeapValue(it->second->leapValue());
        }
    }
     */
}