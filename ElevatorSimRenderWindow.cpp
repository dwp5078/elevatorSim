/*
* Copyright (c) 2012, Joseph Max DeLiso
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* The views and conclusions contained in the software and documentation are those
* of the authors and should not be interpreted as representing official policies,
* either expressed or implied, of the FreeBSD Project.
*/

#include "ElevatorSim.hpp"
#include "ElevatorSimRenderWindow.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>

namespace elevatorSim {

   const int ElevatorSimRenderWindow::LEFT_MARGIN = 120;
   const int ElevatorSimRenderWindow::RIGHT_MARGIN = 8;
   const int ElevatorSimRenderWindow::TOP_MARGIN = 28;
   const int ElevatorSimRenderWindow::BOTTOM_MARGIN = 8;

   const float ElevatorSimRenderWindow::MOVE = 0.5f;

   ElevatorSimRenderWindow::ElevatorSimRenderWindow(int X, int Y, int W, int H, const char* Label) :
   Fl_Gl_Window(X, Y, W, H, Label) {

      spin = 0.0;
      m_vecCamPos.x = 0.f;
      m_vecCamPos.y = 0.f;
      m_vecCamPos.z = 20.f;

      m_vecCamLookAt.x = 0.f;
      m_vecCamLookAt.y = 0.f;
      m_vecCamLookAt.z = 0.f;

      m_vecCamUp.x = 0.0f;
      m_vecCamUp.y = 1.0f;
      m_vecCamUp.z = 0.0f;

      Fl::add_timeout(FPS, timerCB, (void*)this);
   }

   void ElevatorSimRenderWindow::draw() {
      if(!valid()) {
         /* initialize, this code only gets executed the first time draw() is called */

         initCube();
         glInit();
         setViewport();
      }

      /* draw */
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      gluPerspective(45.0f, (GLfloat)w()/(GLfloat)h(), 0.1f, 500.0f);

      gluLookAt(	m_vecCamPos.x, m_vecCamPos.y, m_vecCamPos.z, 
         m_vecCamLookAt.x, m_vecCamLookAt.y, m_vecCamLookAt.z, 
         m_vecCamUp.x, m_vecCamUp.y, m_vecCamUp.z);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      glEnable(GL_LIGHTING);	
      glEnable(GL_LIGHT0);

      glTranslatef(0.0f, 0.0f, -2.0f);
      glRotatef(spin, 0.5, 1.0, 0.0);

      glCallList(OBJ_CUBE);

      /* Render floor */
      glLoadIdentity();

      glTranslatef(0.0f, -2.0f, 0.0f);
      glScalef(10.0f, 10.0f, 10.0f);

      glDisable(GL_LIGHTING);	
      glDisable(GL_LIGHT0);

      glBegin(GL_QUADS);
      glColor3f(0.8f, 0.8f, 0.8f);
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f( 1.0f, 0.0f, -1.0f);
      glVertex3f(-1.0f, 0.0f, -1.0f);
      glVertex3f(-1.0f, 0.0f,  1.0f);
      glVertex3f( 1.0f, 0.0f,  1.0f);
      glEnd();

      GLenum err = glGetError();
      if ( err != GL_NO_ERROR ) {
         /* 
         * TODO: 
         * fprintf(stderr, "GLGETERROR=%d\n", (int)err); 
         */
      }
   }

   void ElevatorSimRenderWindow::glInit() {
      /* if GlInit is called while valid() returns true, drop a breakpoint */
      assert(!valid()); 

      glEnable(GL_TEXTURE_2D);

      glShadeModel(GL_SMOOTH);
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glDepthMask(GL_TRUE);

      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180);

      glLightfv(GL_LIGHT0, GL_AMBIENT, light1_ambient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse);
      glLightfv(GL_LIGHT0, GL_SPECULAR, light1_specular);
      glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
      glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light1_direction);
   }

   void ElevatorSimRenderWindow::setViewport() {
      glViewport(0, 0, w(), h());

      float ratio = (float)w() / (float)h();

      setPerspective(45.0, 1.0*ratio, 1.0, 200.0);
   }

   void ElevatorSimRenderWindow::setPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
      GLdouble xmin, xmax, ymin, ymax;

      ymax = zNear * tan(fovy * M_PI / 360.0);
      ymin = -ymax;
      xmin = ymin * aspect;
      xmax = ymax * aspect;

      glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
   }

   void ElevatorSimRenderWindow::initCube() {
      assert(!valid());

      glNewList(OBJ_CUBE, GL_COMPILE);
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);

      glBegin(GL_QUADS);

      /* Front Face */
      glColor3f(1.0, 0.0, 0.0); /* red */
      glNormal3f(0.0, 0.0, 1.0);
      glVertex3f(-1.0,  1.0,  1.0);
      glVertex3f( 1.0,  1.0,  1.0);
      glVertex3f( 1.0, -1.0,  1.0);
      glVertex3f(-1.0, -1.0,  1.0);

      /* Back Face */
      glColor3f(0.0, 1.0, 1.0); /* cyn */
      glNormal3f(0.0, 0.0, -1.0);
      glVertex3f( 1.0,  1.0, -1.0);
      glVertex3f(-1.0,  1.0, -1.0);
      glVertex3f(-1.0, -1.0, -1.0);
      glVertex3f( 1.0, -1.0, -1.0);

      /* Top Face */
      glColor3f(0.0, 1.0, 0.0); /* grn */
      glNormal3f(0.0, 1.0, 0.0);
      glVertex3f(-1.0,  1.0, -1.0);
      glVertex3f( 1.0,  1.0, -1.0);
      glVertex3f( 1.0,  1.0,  1.0);
      glVertex3f(-1.0,  1.0,  1.0);

      /* Bottom Face */
      glColor3f(1.0, 0.0, 1.0); /* mag */
      glNormal3f(0.0, -1.0, 0.0);
      glVertex3f( 1.0, -1.0, -1.0);
      glVertex3f(-1.0, -1.0, -1.0);
      glVertex3f(-1.0, -1.0,  1.0);
      glVertex3f( 1.0, -1.0,  1.0);

      /* Right face */
      glColor3f(0.0, 0.0, 1.0); /* blu */
      glNormal3f(1.0, 0.0, 0.0);
      glVertex3f( 1.0,  1.0,  1.0);
      glVertex3f( 1.0,  1.0, -1.0);
      glVertex3f( 1.0, -1.0, -1.0);
      glVertex3f( 1.0, -1.0,  1.0);

      /* Left Face */
      glColor3f(1.0, 1.0, 0.0); /* yel */
      glNormal3f(-1.0, 0.0, 0.0);
      glVertex3f(-1.0,  1.0, -1.0);
      glVertex3f(-1.0,  1.0,  1.0);
      glVertex3f(-1.0, -1.0,  1.0);
      glVertex3f(-1.0, -1.0, -1.0);
      glEnd();

      glEndList();
   }

   static void timerCB(void* userdata) {
      ElevatorSimRenderWindow* myWindow = (ElevatorSimRenderWindow*)userdata;

      myWindow->spin += 2.0; /* spin */
      myWindow->redraw();

      Fl::repeat_timeout(FPS, timerCB, userdata);
   }
} /* namespace elevatorSim */