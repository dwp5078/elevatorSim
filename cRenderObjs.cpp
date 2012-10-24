#include "cRenderObjs.hpp"

namespace elevatorSim   {
cRenderObjs::cRenderObjs(void)
{
   m_bIsInit= false;
}

cRenderObjs::~cRenderObjs(void)
{
}
void cRenderObjs::Init()
{
   if(!m_bIsInit) {
      m_bIsInit = true;
      InitPlane();
      InitCube();
      InitBuilding();
      InitElevator();
   }
}

void cRenderObjs::InitPlane()
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

void cRenderObjs::InitCube()
{
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

void cRenderObjs::InitBuilding()
{
   glNewList(OBJ_BUILDING, GL_COMPILE);

   glEndList();
}

void cRenderObjs::InitElevator()
{
   glNewList(OBJ_ELEVATOR, GL_COMPILE);
   glEndList();
}
}
