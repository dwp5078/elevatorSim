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
#include "IPersonCarrier.hpp"
#include "Floor.hpp"
#include "cRenderObjs.hpp"
#include "Person.hpp"
#include "Building.hpp"
#include "SimulationState.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>

namespace elevatorSim {

/* public static constants */
const int Floor::YVALS_PER_FLOOR = 5040;

/* constructors */
Floor::Floor(
   int _yVal,
   int _thisFloor,
   float _gfxScaleWidth,
   bool _hasUpperFloor,
   bool _hasLowerFloor
   ) :
      Location(_yVal),
      thisFloor(_thisFloor),
      gfxScaleWidth(_gfxScaleWidth),
      hasUpperFloor(_hasUpperFloor),
      hasLowerFloor(_hasLowerFloor)  {

      signalingUp = false;
      signalingDown = false;

      if(isDebugBuild()) {
         std::stringstream dbgSS;
         dbgSS << "constructed floor @" << this << std::endl;
         LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer(dbgSS) );
      }
}

Floor::~Floor() {
   init();

   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "destructing floor @" << this << std::endl;
      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer(dbgSS) );
   }
}

void Floor::init() {
   /* free all the people allocated on the heap */
   signalingUp = false;
   signalingDown = false;
}

void Floor::render() {
   GLfloat floor_amb[4] = {0.1f, 0.1f, 0.1f, 1.0f};
   GLfloat floor_dif[4] = {0.5f, 0.5f, 0.5f, 1.0f};
   GLfloat floor_spe[4] = {0.2f, 0.2f, 0.2f, 1.0f};
   GLfloat floor_shi = 0.5f;
   GLfloat floor_emi[4] = {0.0f, 0.0f, 0.0f, 1.0f};

   glMaterialfv(GL_FRONT, GL_AMBIENT, floor_amb);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, floor_dif);
   glMaterialfv(GL_FRONT, GL_SPECULAR, floor_spe);
   glMaterialf(GL_FRONT, GL_SHININESS, floor_shi);
   glMaterialfv(GL_FRONT, GL_EMISSION, floor_emi);

   glPushMatrix();
   glScalef(gfxScaleWidth + cRenderObjs::GFX_FLOOR_QUEUE_SCALE_WIDTH, 0.1f, 2.0f);
   glCallList(cRenderObjs::OBJ_CUBE);
   glPopMatrix();

   /* Render Arrow */
   {
      glPushMatrix();
      glTranslatef(-gfxScaleWidth + 0.8f, 1.1f, 0.2f);
      glScalef(0.25f, 0.25f, 0.25f);

      if(signalingUp)   {
         GLfloat arrow_dif[4] = {0.9f, 0.0f, 0.0f, 1.0f};
         glMaterialfv(GL_FRONT, GL_DIFFUSE, arrow_dif);
      }

      else  {
         GLfloat arrow_dif[4] = {0.1f, 0.1f, 0.1f, 1.0f};
         glMaterialfv(GL_FRONT, GL_DIFFUSE, arrow_dif);
      }

      glBegin(GL_TRIANGLES);
      glNormal3f(0.0, 1.0f, 1.0f);
      glVertex3f(1.0,  0.0,  0.0);
      glVertex3f( -1.0,  0.0,  0.0);
      glVertex3f( 0.0, 1.73f,  0.0);
      glEnd();
      glPopMatrix();

      glPushMatrix();
      glTranslatef(-gfxScaleWidth+0.8, 0.9f, 0.2f);
      glScalef(0.25f, 0.25f, 0.25f);

      if(signalingDown) {
         GLfloat arrow_dif[4] = {0.0f, 0.9f, 0.0f, 1.0f};
         glMaterialfv(GL_FRONT, GL_DIFFUSE, arrow_dif);
      }

      else  {
         GLfloat arrow_dif[4] = {0.1f, 0.1f, 0.1f, 1.0f};
         glMaterialfv(GL_FRONT, GL_DIFFUSE, arrow_dif);
      }

      glBegin(GL_TRIANGLES);
      glNormal3f(0.0, 1.0f, 1.0f);
      glVertex3f( -1.0,  0.0,  0.0);
      glVertex3f(1.0,  0.0,  0.0);
      glVertex3f( 0.0, -1.73f,  0.0);
      glEnd();
      glPopMatrix();
   }

   glPushMatrix();
   glTranslatef(-gfxScaleWidth-0.5f, 1.0f, 0.f);
   cRenderObjs::renderOccupants(numPeopleContained(), 50);
   glPopMatrix();
}

void Floor::update() {
   updateSignalArrows();

   /* remove occupants from this floor who are destined here.
    * (OWNER FREES+CHILD MOVES) */

   /* this is a very special iteration over the set because 
    * the keys are liable to be deleted during execution 
    * of functions called as a result of person update.
    */
   for(std::unordered_set<Person*>::iterator iter = people.begin();
      iter != people.end();
      ) {
         /* obtain a pointer to the current person by using iterator */
         Person* currentMutablePerson = *iter;

         /* copy construct an iterator from the current one */
         std::unordered_set<Person*>::iterator nextPosition = std::unordered_set<Person*>::iterator(iter);

         /* increment iterator position, to save the next position */
         ++nextPosition;

         /* if the person was getting off on this floor, remove them from the simulation */
         if(currentMutablePerson->getDestination().getYVal()
            == thisFloor * Floor::YVALS_PER_FLOOR )  {

               /* pass the old iteration position to erase, but first jump to a new one */
               people.erase(iter++);

               /* free the mutable person from the simulation */
               delete currentMutablePerson;
         } else {
            /* otherwise we just update the person */
            currentMutablePerson -> update();

            /* the current iterator could've been invalidated by a person moving itself, 
             * so intead of iter++ we just overwrite with the saved position */
            iter = nextPosition;
         }
   }
}

void Floor::updateSignalArrows() {
   signalingUp = false;
   signalingDown = false;

   std::unordered_set<Person*>::const_iterator iter = people.begin();
   while(iter != people.end()) {
      const Person* currentPerson = *iter;
      if(currentPerson->getDestination().getYVal()
         - thisFloor > 0) {
            signalingUp = true;
      } else if(currentPerson->getDestination().getYVal()
         - thisFloor < 0) {
            signalingDown = true;
      }

      iter++;
   }
}

} /* namespace elevatorSim */
