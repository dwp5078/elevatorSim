/*
 * Copyright (c) 2012, Joseph Max DeLiso, Daniel Gilbert
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
#include "Building.hpp"
#include "Floor.hpp"
#include "Elevator.hpp"
#include "cRenderObjs.hpp"

namespace elevatorSim {
/* constructors */
Building::Building(unsigned int nStory, unsigned int nElevator) {
   m_nStory = nStory;
   m_nElevator = nElevator;

   m_Floors = new Floor[m_nStory];
   m_Elevators = new Elevator[m_nElevator];
}

Building::~Building() {
   delete [] m_Floors;
   delete [] m_Elevators;
}

/* public methods inherited from SimulationTerminal*/
void Building::init() {
   for(unsigned int i=0; i<m_nStory-1; i++) {
      m_Floors[i] = Floor(i * Floor::YVALS_PER_FLOOR, i != m_nStory-1, i != 0 );
   }
   
   /* TOOD: intialize each elevator */
}

void Building::render()
{
   const GLfloat gfxScaleHeight = m_nStory * cRenderObjs::BUILDING_GAP_HEIGHT;
   const GLfloat gfxScaleWidth = m_nElevator * cRenderObjs::ELEV_GAP_WIDTH;

   glLoadIdentity();
   glTranslatef(0.0f, -2.0f, 0.0f);

   /* Left wall */
   glPushMatrix();
   glTranslatef(-gfxScaleWidth, gfxScaleHeight, 0.f);
   glScalef(0.1f, gfxScaleHeight, 2.0f);
   glCallList(cRenderObjs::OBJ_CUBE);
   glPopMatrix();

   /* Right wall */
   glPushMatrix();
   glTranslatef(gfxScaleWidth, gfxScaleHeight, 0.f);
   glScalef(0.1f, gfxScaleHeight, 2.0f);
   glCallList(cRenderObjs::OBJ_CUBE);
   glPopMatrix();

   /* Back wall */
   glPushMatrix();
   glTranslatef(0, gfxScaleHeight, -2.0f);
   glScalef(gfxScaleWidth, gfxScaleHeight, 0.1f);
   glCallList(cRenderObjs::OBJ_CUBE);
   glPopMatrix();

   /* Top wall */
   glPushMatrix();
   glTranslatef(0, gfxScaleHeight*2, 0.0f);
   glScalef(gfxScaleWidth, 0.1f, 2.0f);
   glCallList(cRenderObjs::OBJ_CUBE);
   glPopMatrix();

   /* FIXME: reconsider the relationship between eachHeight and the floor's yVal */
   float eachHeight = gfxScaleHeight * 2 / m_nStory;

   /* Draw each floor */
   for(unsigned int i=0; i<m_nStory-1; i++) {
      glPushMatrix();
      glTranslatef(0.0f, eachHeight * (i+1), 0.f);
      glScalef(gfxScaleWidth, 0.1f, 2.0f);

      m_Floors[i].render();

      glPopMatrix();
   }

   /* TODO: draw each elevator */
   for(unsigned int i = 0; i < m_nElevator; i++) {
      m_Elevators[i].render();
   }

   /* Render land */
   glPushMatrix();
   glScalef(10.0f, 10.0f, 10.0f);
   glCallList(cRenderObjs::OBJ_PLANE);
   glPopMatrix();
}

void Building::update()
{
   for(unsigned int i = 0; i < m_nStory; i++) {
      m_Floors[i].update();
   }

   for(unsigned int i = 0; i < m_nElevator; i++) {
      m_Elevators[i].update();
   }
}

} /* namepsace elevatorSim */
