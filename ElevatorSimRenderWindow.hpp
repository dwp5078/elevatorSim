/*
 * Copyright (c) 2012, Joseph Max DeLiso, Daniel Gilbert
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the FreeBSD Project.
 */

#ifndef _ELEVATOR_SIM_RENDER_WINDOW_H
#define _ELEVATOR_SIM_RENDER_WINDOW_H

#include "ElevatorSim.hpp"
#include "cCameraManager.hpp"
#include "cTimeManager.hpp"
#include "cKeyManager.hpp"
#include "cRenderObjs.hpp"
#include "Building.hpp"
#include "MVectors.hpp"

#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>

namespace elevatorSim {

class ElevatorSimRenderWindow : public Fl_Gl_Window {

   /* friends */
   friend class ElevatorSimWindow;

   /* private static methods */
   static void timerCB(void* userData);

   /* private static constants */
   static const GLfloat light1_ambient[4];
   static const GLfloat light1_diffuse[4];
   static const GLfloat light1_specular[4];
   static const GLfloat light1_position[4];
   static const GLfloat light1_direction[4];

   /* private instance members */
   float spin;
   int GLWindow_width;
   int GLWindow_height;

   /* private methods */
   int handle(int event);
   void glInit();
   void setViewport();
   void setPerspective(
         GLdouble fovy,
         GLdouble aspect,
         GLdouble zNear,
         GLdouble zFar);

   void drawFPS(int fps, int totalFrames);
   void drawText(const char * const str, float x, float y);
   void rayCasting(int x, int y);

   /* public methods */
   ElevatorSimRenderWindow(
      int X, int Y, int W, int H, const char* Label = 0);

public:

   /* public static constants */
   static const int LEFT_MARGIN;
   static const int RIGHT_MARGIN;
   static const int TOP_MARGIN;
   static const int BOTTOM_MARGIN;

   bool m_bRenderFPS;

   /* public instance members */

   ~ElevatorSimRenderWindow();
   
   void draw();
};

} /* namespace elevatorSim */

#endif
