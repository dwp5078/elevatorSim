#include "cRenderObjs.hpp"

namespace elevatorSim   {
   cRenderObjs::cRenderObjs(void)
   {
      m_bIsInit= false;
      m_nNumberOfFloor = 10;
      m_nNumberOfElevator = g_nNumberOfElev;
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

      float scaleHeight = m_nNumberOfFloor * 1.1f;
      float scaleWidth = m_nNumberOfElevator * ELEV_GAP_WIDTH;

      glPushMatrix();
      // Left wall
      glTranslatef(-scaleWidth, scaleHeight, 0.f);
      glScalef(0.1f, scaleHeight, 2.0f);
      glCallList(OBJ_CUBE);
      glPopMatrix();

      // Right wall
      glPushMatrix();
      glTranslatef(scaleWidth, scaleHeight, 0.f);
      glScalef(0.1f, scaleHeight, 2.0f);
      glCallList(OBJ_CUBE);
      glPopMatrix();

      // Back wall
      glPushMatrix();
      glTranslatef(0, scaleHeight, -2.0f);
      glScalef(scaleWidth, scaleHeight, 0.1f);
      glCallList(OBJ_CUBE);
      glPopMatrix();

      // Top wall
      glPushMatrix();
      glTranslatef(0, scaleHeight*2, 0.0f);
      glScalef(scaleWidth, 0.1f, 2.0f);
      glCallList(OBJ_CUBE);
      glPopMatrix();

      // Draw each floor

      for(int i=0; i<m_nNumberOfFloor-1; i++)
      {
         float eachHeight = scaleHeight * 2 / m_nNumberOfFloor;
         glPushMatrix();
         glTranslatef(0.0f, eachHeight * (i+1), 0.f);
         glScalef(scaleWidth, 0.1f, 2.0f);
         glCallList(OBJ_CUBE);
         glPopMatrix();
      }

      // Render land
      glPushMatrix();
      glScalef(10.0f, 10.0f, 10.0f);
      glCallList(OBJ_PLANE);
      glPopMatrix();

      glEndList();
   }

   void cRenderObjs::InitElevator()
   {
      glNewList(OBJ_ELEVATOR, GL_COMPILE);

      float scaleHeight = 1.0f;
      float scaleWidth = 0.75f;

      glPushMatrix();
      // Left wall of Elevator
      glTranslatef(-scaleWidth, 0, 0.f);
      glScalef(0.05f, scaleHeight, 1.0f);
      glCallList(OBJ_CUBE);
      glPopMatrix();

      // Right wall of Elevator
      glPushMatrix();
      glTranslatef(scaleWidth, 0, 0.f);
      glScalef(0.05f, scaleHeight, 1.0f);
      glCallList(OBJ_CUBE);
      glPopMatrix();

      // Back wall of Elevator
      glPushMatrix();
      glTranslatef(0, 0, -1.0f);
      glScalef(scaleWidth, scaleHeight, 0.05f);
      glCallList(OBJ_CUBE);
      glPopMatrix();

      // Top wall of Elevator
      glPushMatrix();
      glTranslatef(0, scaleHeight, 0.0f);
      glScalef(scaleWidth, 0.05f, 1.0f);
      glCallList(OBJ_CUBE);
      glPopMatrix();

      // Bottom wall of Elevator
      glPushMatrix();
      glTranslatef(0, -scaleHeight, 0.0f);
      glScalef(scaleWidth, 0.05f, 1.0f);
      glCallList(OBJ_CUBE);
      glPopMatrix();

      glEndList();
   }
}