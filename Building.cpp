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

#include <vector>

namespace elevatorSim {
/* constructors */
Building::Building(unsigned int nStory, unsigned int nElevator) {
   m_nStory = nStory;
   m_nElevator = nElevator;
}

/* public methods inherited from SimulationTerminal*/
void Building::init() {

}

void Building::render()
{
   const GLfloat scaleHeight = m_nStory * cRenderObjs::BUILDING_GAP_HEIGHT;
   const GLfloat scaleWidth = m_nElevator * cRenderObjs::ELEV_GAP_WIDTH;

   glLoadIdentity();
   glTranslatef(0.0f, -2.0f, 0.0f);

   glPushMatrix();
   // Left wall
   glTranslatef(-scaleWidth, scaleHeight, 0.f);
   glScalef(0.1f, scaleHeight, 2.0f);
   glCallList(cRenderObjs::OBJ_CUBE);
   glPopMatrix();

   // Right wall
   glPushMatrix();
   glTranslatef(scaleWidth, scaleHeight, 0.f);
   glScalef(0.1f, scaleHeight, 2.0f);
   glCallList(cRenderObjs::OBJ_CUBE);
   glPopMatrix();

   // Back wall
   glPushMatrix();
   glTranslatef(0, scaleHeight, -2.0f);
   glScalef(scaleWidth, scaleHeight, 0.1f);
   glCallList(cRenderObjs::OBJ_CUBE);
   glPopMatrix();

   // Top wall
   glPushMatrix();
   glTranslatef(0, scaleHeight*2, 0.0f);
   glScalef(scaleWidth, 0.1f, 2.0f);
   glCallList(cRenderObjs::OBJ_CUBE);
   glPopMatrix();

   // Draw each floor
   for(unsigned int i=0; i<m_nStory-1; i++)
   {
      float eachHeight = scaleHeight * 2 / m_nStory;
      glPushMatrix();
      glTranslatef(0.0f, eachHeight * (i+1), 0.f);
      glScalef(scaleWidth, 0.1f, 2.0f);
      glCallList(cRenderObjs::OBJ_CUBE);
      glPopMatrix();
   }

   // Render land
   glPushMatrix();
   glScalef(10.0f, 10.0f, 10.0f);
   glCallList(cRenderObjs::OBJ_PLANE);
   glPopMatrix();
   

   /* does nothing yet */
   for(unsigned int i = 0; i < m_Floors.size(); i++) {
      m_Floors[i].render();
   }

   /* does nothing yet */
   for(unsigned int i = 0; i < m_Elevators.size(); i++) {
      m_Elevators[i].render();
   }
}

void Building::update()
{
   for(unsigned int i = 0; i < m_Floors.size(); i++) {
      m_Floors[i].update();
   }

   for(unsigned int i = 0; i < m_Elevators.size(); i++) {
      m_Elevators[i].update();
   }
}

/*Building::Building(const std::vector<Floor> & floorSetup, const std::vector<Elevator> & elevatorSetup) {
   floors.assign(floorSetup.begin(), floorSetup.end());
   elevators.assign(elevatorSetup.begin(), elevatorSetup.end());
   }*/
}
