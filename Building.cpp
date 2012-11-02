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

#include <GL/glut.h>
#include <iostream>

namespace elevatorSim {
/* constructors */
Building::Building(unsigned int _nStory, unsigned int _nElevator) :
   m_nStory(_nStory),
   m_nElevator(_nElevator),
   gfxScaleHeight(_nStory * cRenderObjs::BUILDING_GAP_HEIGHT),
   gfxScaleWidth(_nElevator * cRenderObjs::ELEV_GAP_WIDTH),
   gfxEachFloorHeight(gfxScaleHeight * 2 / _nStory ),
   gfxEachElevWidth(gfxScaleWidth * 2 / _nElevator) {

      m_Floors = new Floor * [m_nStory];
      m_Elevators = new Elevator * [m_nElevator];

      for(unsigned int i=0; i < m_nStory ; ++i) {
         m_Floors[i] = new Floor(*this, i * Floor::YVALS_PER_FLOOR, i != m_nStory-1, i != 0 );
      }

      for(unsigned int i=0; i < m_nElevator ; ++i ) {
         m_Elevators[i] = new Elevator(*this, 0);
      }
}

Building::Building(const Building& buildingCopy) :
   m_nStory(buildingCopy.m_nStory),
   m_nElevator(buildingCopy.m_nElevator),
   gfxScaleHeight(buildingCopy.m_nStory * cRenderObjs::BUILDING_GAP_HEIGHT),
   gfxScaleWidth(buildingCopy.m_nElevator * cRenderObjs::ELEV_GAP_WIDTH),
   gfxEachFloorHeight(gfxScaleHeight * 2 / buildingCopy.m_nStory ),
   gfxEachElevWidth(gfxScaleWidth * 2 / buildingCopy.m_nElevator) {

      if(isDebugBuild()) {
         std::cout << "in copy constructor for building @" << this << std::endl;
      }

      /* TODO: finish this */
}


Building::~Building() {
   for(unsigned int i=0; i < m_nStory ; ++i) {
      delete m_Floors[i];
   }

   for(unsigned int i=0; i < m_nElevator ; ++i ) {
      delete m_Elevators[i];
   }

   delete [] m_Floors;
   delete [] m_Elevators;
}

/* public methods inherited from SimulationTerminal */

void Building::init() {
   for(unsigned int i=0; i < m_nStory ; ++i) {
      m_Floors[i]->init();
   }
   
   for(unsigned int i=0; i < m_nElevator ; ++i ) {
      m_Elevators[i]->init();
   }
}

void Building::render() {
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

   /* Draw each floor */
   for(unsigned int i=0; i < m_nStory - 1; i++) {
      glPushMatrix();
      glTranslatef(0.0f, gfxEachFloorHeight * (i+1), 0.f);
      glScalef(gfxScaleWidth, 0.1f, 2.0f);

      m_Floors[i]->render();

      glPopMatrix();
   }

   /* Draw each elevator */
   for(unsigned int i=0; i < m_nElevator; i++) {
      glPushMatrix();
      glTranslatef(
         -gfxScaleWidth + cRenderObjs::ELEV_GAP_WIDTH + gfxEachElevWidth * i, 
         1.0f, 
         0.0f);

      /*
       * elev height is on interval [1.0f, 1.0f + (m_nElevator - 1) * gfxEachFloorHeight] 
       */

      m_Elevators[i]->render();
      glPopMatrix();

   }

   /* Render land */
   glPushMatrix();
   glScalef(10.0f, 10.0f, 10.0f); /* FIXME: this needs to scale past 5 elevators */
   glCallList(cRenderObjs::OBJ_PLANE);
   glPopMatrix();
}

void Building::update()
{
   for(unsigned int i = 0; i < m_nStory; i++) {
      m_Floors[i]->update();
   }

   for(unsigned int i = 0; i < m_nElevator; i++) {
      m_Elevators[i]->update();
   }
}

} /* namepsace elevatorSim */
