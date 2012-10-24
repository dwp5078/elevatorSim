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
#include "cTimeManager.hpp"
#include "cKeyManager.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <time.h>

namespace elevatorSim {

const int ElevatorSimRenderWindow::LEFT_MARGIN = 120;
const int ElevatorSimRenderWindow::RIGHT_MARGIN = 8;
const int ElevatorSimRenderWindow::TOP_MARGIN = 28;
const int ElevatorSimRenderWindow::BOTTOM_MARGIN = 8;

const GLfloat ElevatorSimRenderWindow::light1_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
const GLfloat ElevatorSimRenderWindow::light1_diffuse[] = { 0.15f, 0.15f, 0.15f, 1.0f };
const GLfloat ElevatorSimRenderWindow::light1_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat ElevatorSimRenderWindow::light1_position[] = { 1.f, 8.f, 2.0f, 0.0f };
const GLfloat ElevatorSimRenderWindow::light1_direction[] = { 0.0f, 0.0f, 0.0f, 0.0f };

void ElevatorSimRenderWindow::timerCB(void* userdata) {
   ElevatorSimRenderWindow* myWindow = (ElevatorSimRenderWindow*)userdata;

   myWindow->Update();
   myWindow->redraw();

   Fl::repeat_timeout(myWindow->getFPS(), timerCB, userdata);
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

void ElevatorSimRenderWindow::Update()
{
   cTimeManager::GetInstance()->Update();

   if(cKeyManager::GetInstance()->onceKeyDown('F')){
      m_bRenderFPS = !m_bRenderFPS;
   }

   m_CameraManager.Update();
}

void ElevatorSimRenderWindow::setViewport() {
   glViewport(0, 0, w(), h());

   float ratio = (float)w() / (float)h();

   setPerspective(45.0, 1.0*ratio, 1.0, 200.0);
}

void ElevatorSimRenderWindow::setPerspective(
      GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
   GLdouble xmin, xmax, ymin, ymax;

   ymax = zNear * tan(fovy * M_PI / 360.0);
   ymin = -ymax;
   xmin = ymin * aspect;
   xmax = ymax * aspect;

   glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

void ElevatorSimRenderWindow::drawFPS()
{
   const static int renderStringBufferLength = 256;
   char* renderStringBuffer = (char*) malloc(renderStringBufferLength);

   glColor3f(0.0f, 1.f, 0.f); /* because green text is sexy text */


   snprintf(renderStringBuffer,
         renderStringBufferLength, 
         "Total Frame : %d", 
         cTimeManager::GetInstance()->GetTotalFrame());

   drawText(renderStringBuffer, 10.f, 10.f);

   snprintf(renderStringBuffer,
         renderStringBufferLength, 
         "FPS : %d", 
         cTimeManager::GetInstance()->GetFPS());

   drawText(renderStringBuffer, 10.f, 20.f);

   snprintf(renderStringBuffer,
         renderStringBufferLength, 
         "Elapsed Time : %d", 
         cTimeManager::GetInstance()->GetElapsedTime());

   drawText(renderStringBuffer, 10.f, 30.f);

   snprintf(renderStringBuffer,
         renderStringBufferLength, 
         "World Time : %d", 
         cTimeManager::GetInstance()->GetWorldTime());

   drawText(renderStringBuffer, 10.f, 40.f);

   free(renderStringBuffer);
}

void ElevatorSimRenderWindow::drawText(const char * const str, float x, float y) {
   /* enabling prolog */
   glDisable(GL_DEPTH_TEST);
   glPushAttrib(GL_LIGHTING_BIT);
   glDisable(GL_LIGHTING);

   /* projection push */
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();

   /* modelview push */
   glLoadIdentity();
   gluOrtho2D(0, w(), 0, h());
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();

   glLoadIdentity();
   glRasterPos2f(x, y);

   /* loop through the string and draw each character */
   for (const char *c = str; *c != '\0'; c++) {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
   }

   /* modelview pop */
   glPopMatrix();

   /* projection pop */
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();

   /* disabling prolog */
   glMatrixMode(GL_MODELVIEW);
   glPopAttrib();
   glEnable(GL_DEPTH_TEST);
}

ElevatorSimRenderWindow::ElevatorSimRenderWindow(
      int X, int Y, int W, int H, const char* Label) :
                                 Fl_Gl_Window(X, Y, W, H, Label) {

   spin = 0.0;
   FPS = 38;
   m_bRenderFPS = true;

   Fl::add_timeout(FPS, timerCB, (void*)this);
}

void ElevatorSimRenderWindow::draw() {
   if(!valid()) {
      /* initialize, this code only gets executed the first time draw() is called */

      m_renderObjs.Init();
      glInit();
      setViewport();
   }

   /* draw */
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   /* TODO: make these constants somewhere */
   gluPerspective(45.0f, (GLfloat)w()/(GLfloat)h(), 0.1f, 500.0f);

   m_CameraManager.Render();

   glMatrixMode(GL_MODELVIEW);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   /* Draw building object */
   glLoadIdentity();
   glTranslatef(0.0f, -2.0f, 0.0f);
   glCallList(OBJ_BUILDING);

   if(m_bRenderFPS) { drawFPS(); }

   GLenum err = glGetError();
   if ( err != GL_NO_ERROR ) {
      fprintf(stderr, "GLGETERROR=%d\n", (int)err);
   }
}

} /* namespace elevatorSim */
