/*
 * Copyright (c) 2012, Joseph Max DeLiso
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

#include "ElevatorSim.hpp"
#include "cRenderObjs.hpp"

namespace elevatorSim   {

const GLfloat cRenderObjs::BUILDING_GAP_HEIGHT = 1.1f;
const GLfloat cRenderObjs::ELEV_GAP_WIDTH = 1.4f;
const GLfloat cRenderObjs::GFX_ELEV_SCALE_HEIGHT = 1.0f;
const GLfloat cRenderObjs::GFX_ELEV_SCALE_WIDTH = 0.75f;
const GLfloat cRenderObjs::GFX_FLOOR_QUEUE_SCALE_WIDTH = 2.f;

bool cRenderObjs::m_bIsInit = false;

cRenderObjs::cRenderObjs() {

}

void cRenderObjs::init()
{
   if(!m_bIsInit) {
      m_bIsInit = true;
      initPlane();
      initCube();
      initSphere();
      initElevator();
      initHuman();
   }
}

void cRenderObjs::initPlane()
{
   glNewList(OBJ_PLANE, GL_COMPILE);
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

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   glEndList();
}

void cRenderObjs::initCube()
{
   glNewList(OBJ_CUBE, GL_COMPILE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   glBegin(GL_QUADS);

   /* Front Face */
   glNormal3f(0.0, 0.0, 1.0);
   glVertex3f(-1.0,  1.0,  1.0);
   glVertex3f( 1.0,  1.0,  1.0);
   glVertex3f( 1.0, -1.0,  1.0);
   glVertex3f(-1.0, -1.0,  1.0);

   /* Back Face */
   glNormal3f(0.0, 0.0, -1.0);
   glVertex3f( 1.0,  1.0, -1.0);
   glVertex3f(-1.0,  1.0, -1.0);
   glVertex3f(-1.0, -1.0, -1.0);
   glVertex3f( 1.0, -1.0, -1.0);

   /* Top Face */
   glNormal3f(0.0, 1.0, 0.0);
   glVertex3f(-1.0,  1.0, -1.0);
   glVertex3f( 1.0,  1.0, -1.0);
   glVertex3f( 1.0,  1.0,  1.0);
   glVertex3f(-1.0,  1.0,  1.0);

   /* Bottom Face */
   glNormal3f(0.0, -1.0, 0.0);
   glVertex3f( 1.0, -1.0, -1.0);
   glVertex3f(-1.0, -1.0, -1.0);
   glVertex3f(-1.0, -1.0,  1.0);
   glVertex3f( 1.0, -1.0,  1.0);

   /* Right face */
   glNormal3f(1.0, 0.0, 0.0);
   glVertex3f( 1.0,  1.0,  1.0);
   glVertex3f( 1.0,  1.0, -1.0);
   glVertex3f( 1.0, -1.0, -1.0);
   glVertex3f( 1.0, -1.0,  1.0);

   /* Left Face */
   glNormal3f(-1.0, 0.0, 0.0);
   glVertex3f(-1.0,  1.0, -1.0);
   glVertex3f(-1.0,  1.0,  1.0);
   glVertex3f(-1.0, -1.0,  1.0);
   glVertex3f(-1.0, -1.0, -1.0);
   glEnd();

   glEndList();
}

void cRenderObjs::initSphere()
{
   glNewList(OBJ_SPHERE, GL_COMPILE);

   glutSolidSphere(0.4, 50, 25);

   glEndList();
}

void cRenderObjs::initElevator()
{
   glNewList(OBJ_ELEVATOR, GL_COMPILE);

   GLfloat amb[4] = {0.1f, 0.1f, 0.1f, 1.0f};
   GLfloat dif[4] = {0.2f, 0.8f, 0.2f, 1.0f};
   GLfloat spe[4] = {0.2f, 0.2f, 0.2f, 1.0f};
   GLfloat shi = 0.3f;
   GLfloat emi[4] = {0.0f, 0.0f, 0.0f, 1.0f};
   glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
   glMaterialfv(GL_FRONT, GL_SPECULAR, spe);
   glMaterialf(GL_FRONT, GL_SHININESS, shi);
   glMaterialfv(GL_FRONT, GL_EMISSION, emi);

   /* Left wall of Elevator */
   glPushMatrix();
   glTranslatef(-GFX_ELEV_SCALE_WIDTH, 0, 0.f);
   glScalef(0.05f, GFX_ELEV_SCALE_HEIGHT, 1.0f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   /* Right wall of Elevator */
   glPushMatrix();
   glTranslatef(GFX_ELEV_SCALE_WIDTH, 0, 0.f);
   glScalef(0.05f, GFX_ELEV_SCALE_HEIGHT, 1.0f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   /* Back wall of Elevator */
   glPushMatrix();
   glTranslatef(0, 0, -1.0f);
   glScalef(GFX_ELEV_SCALE_WIDTH, GFX_ELEV_SCALE_HEIGHT, 0.05f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   /* Top wall of Elevator */
   glPushMatrix();
   glTranslatef(0, GFX_ELEV_SCALE_HEIGHT, 0.0f);
   glScalef(GFX_ELEV_SCALE_WIDTH, 0.05f, 1.0f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   /* Bottom wall of Elevator */
   glPushMatrix();
   glTranslatef(0, -GFX_ELEV_SCALE_HEIGHT, 0.0f);
   glScalef(GFX_ELEV_SCALE_WIDTH, 0.05f, 1.0f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   glEndList();
}

void cRenderObjs::initHuman()
{
   glNewList(OBJ_HUMAN, GL_COMPILE);

   glScalef(0.4f, 0.4f, 0.4f);

   glPushMatrix();
   glTranslatef(0.f, 1.5f, 0.f);
   glCallList(OBJ_SPHERE);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0f, 0.3f, 0.0f);
   glScalef(0.55f, 0.75f, 0.2f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.33f, -1.3f, 0.0f);
   glScalef(0.22f, 0.85f, 0.2f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-0.33f, -1.3f, 0.0f);
   glScalef(0.22f, 0.85f, 0.2f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-.73f, 0.33f, 0.0f);
   glRotatef(-25, 0.0f, 0.0f, 1.0f);
   glScalef(0.17f, 0.7f, 0.17f);

   glCallList(OBJ_CUBE);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(.73f, 0.33f, 0.0f);
   glRotatef(25, 0.0f, 0.0f, 1.0f);
   glScalef(0.17f, 0.7f, 0.17f);

   glCallList(OBJ_CUBE);
   glPopMatrix();

   glEndList();
}

void cRenderObjs::renderOccupants(int num, int maxOccupants, bool forFloor)
{
   if(num > 0)   {
      GLfloat human_amb[4] = {0.1f, 0.1f, 0.1f, 1.0f};
      GLfloat human_dif[4] = {0.1f, 0.1f, 0.1f, 1.0f};
      GLfloat human_spe[4] = {0.2f, 0.2f, 0.2f, 1.0f};
      GLfloat human_shi = 0.5f;
      GLfloat human_emi[4] = {0.0f, 0.0f, 0.0f, 1.0f};

      float perc = (float)num / maxOccupants;

      if(perc > 0.8f)   {
         human_dif[0] = 1.0f, human_dif[1] = 0.0f, human_dif[2] = 0.0f;
      }
      else  {


         human_dif[0] = 0.9f * perc;
         human_dif[2] = 0.9f * (1.0f - perc);
      }

      glMaterialfv(GL_FRONT, GL_AMBIENT, human_amb);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, human_dif);
      glMaterialfv(GL_FRONT, GL_SPECULAR, human_spe);
      glMaterialf(GL_FRONT, GL_SHININESS, human_shi);
      glMaterialfv(GL_FRONT, GL_EMISSION, human_emi);

      float posx = 0.f;

      if(forFloor)   {
         posx = 0.5f;
      }

      else  {
         posx = 0.22f;
      }

      if(perc <= 0.33f) {
         glPushMatrix();
         glCallList(cRenderObjs::OBJ_HUMAN);
         glPopMatrix();
      }

      else if(perc <= 0.66f) {
         glPushMatrix();
         glTranslatef(posx, 0.f, 0.f);
         glCallList(cRenderObjs::OBJ_HUMAN);
         glPopMatrix();

         glPushMatrix();
         glTranslatef(-posx, 0.f, 0.f);
         glCallList(cRenderObjs::OBJ_HUMAN);
         glPopMatrix();
      }

      else{
         glPushMatrix();
         glTranslatef(posx, 0.f, -0.15f);
         glCallList(cRenderObjs::OBJ_HUMAN);
         glPopMatrix();

         glPushMatrix();
         glTranslatef(-posx, 0.f, -0.15f);
         glCallList(cRenderObjs::OBJ_HUMAN);
         glPopMatrix();

         glPushMatrix();
         glTranslatef(0.f, 0.f, 0.15f);
         glCallList(cRenderObjs::OBJ_HUMAN);
         glPopMatrix();
      }
   }
}

void cRenderObjs::drawBitmapText(char *string,float x,float y,float z)
{  
   glDisable(GL_LIGHTING);
   glDisable(GL_LIGHT0);

   char *c;
   glRasterPos3f(x, y, z);

   for (c=string; *c != '\0'; c++) 
   {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
   }

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
}


} /* namespace elevatorSim */
