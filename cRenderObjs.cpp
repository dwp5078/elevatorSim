#include "ElevatorSim.hpp"
#include "cRenderObjs.hpp"

namespace elevatorSim   {

const GLfloat cRenderObjs::BUILDING_GAP_HEIGHT = 1.1f;
const GLfloat cRenderObjs::ELEV_GAP_WIDTH = 1.7f;
const GLfloat cRenderObjs::GFX_ELEV_SCALE_HEIGHT = 1.0f;
const GLfloat cRenderObjs::GFX_ELEV_SCALE_WIDTH = 0.75f;

cRenderObjs::cRenderObjs()
{
   m_bIsInit = false;
}

void cRenderObjs::init()
{
   if(!m_bIsInit) {
      m_bIsInit = true;
      initPlane();
      initCube();
      initElevator();
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

void cRenderObjs::initElevator()
{
   glNewList(OBJ_ELEVATOR, GL_COMPILE);

   glPushMatrix();
   // Left wall of Elevator
   glTranslatef(-GFX_ELEV_SCALE_WIDTH, 0, 0.f);
   glScalef(0.05f, GFX_ELEV_SCALE_HEIGHT, 1.0f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   // Right wall of Elevator
   glPushMatrix();
   glTranslatef(GFX_ELEV_SCALE_WIDTH, 0, 0.f);
   glScalef(0.05f, GFX_ELEV_SCALE_HEIGHT, 1.0f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   // Back wall of Elevator
   glPushMatrix();
   glTranslatef(0, 0, -1.0f);
   glScalef(GFX_ELEV_SCALE_WIDTH, GFX_ELEV_SCALE_HEIGHT, 0.05f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   // Top wall of Elevator
   glPushMatrix();
   glTranslatef(0, GFX_ELEV_SCALE_HEIGHT, 0.0f);
   glScalef(GFX_ELEV_SCALE_WIDTH, 0.05f, 1.0f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   // Bottom wall of Elevator
   glPushMatrix();
   glTranslatef(0, -GFX_ELEV_SCALE_HEIGHT, 0.0f);
   glScalef(GFX_ELEV_SCALE_WIDTH, 0.05f, 1.0f);
   glCallList(OBJ_CUBE);
   glPopMatrix();

   glEndList();
}
}

