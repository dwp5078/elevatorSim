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
#include "Building.hpp"
#include "Floor.hpp"
#include "Elevator.hpp"
#include "cRenderObjs.hpp"
#include "Logger.hpp"

#include <GL/glut.h>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>
#include <cassert>

namespace elevatorSim {

template <class T> PyObject* 
   Building::createTupleFromMember( const std::vector<T*>& memberRef ) {
      /* instantiate a tuple for containing all of the floors */
      PyObject* memberItemTuple = PyTuple_New( memberRef.size() );
   
      if( memberItemTuple == NULL || PyErr_Occurred() ) {
         PyErr_Print();
      }

      assert( memberItemTuple != NULL );

      /* iterate over each floor, telling it to update its tuple */
      int i = 0;
      for( std::vector<T*>::const_iterator iter = memberRef.begin(); 
      iter != memberRef.end();
      ++iter) {
         T* const currentItemGeneric = *iter;
         ISimulationTerminal* currentItemTyped 
            = static_cast<ISimulationTerminal*>(currentItemGeneric);
         currentItemTyped->updateTuple();
         PyTuple_SET_ITEM(memberItemTuple, i, currentItemTyped->getTuple());
         ++i;
      }

      return memberItemTuple;
}

/* constructors */
Building::Building(unsigned int _nStory,
         unsigned int _nElevator,
         int _invPersonArriveProb) :
                  gfxScaleHeight(_nStory * cRenderObjs::BUILDING_GAP_HEIGHT),
                  gfxScaleWidth(_nElevator * cRenderObjs::ELEV_GAP_WIDTH),
                  gfxEachFloorHeight(gfxScaleHeight * 2 / _nStory ),
                  gfxEachElevWidth(gfxScaleWidth * 2 / _nElevator),
                  invPersonArriveProb(_invPersonArriveProb) {

   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "in Building(" << _nStory << ", " << _nElevator
               << ") with address @" << this << std::endl;
      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer( dbgSS ));
   }

   floors = std::vector<Floor*> (_nStory);

   for(unsigned int i=0; i < _nStory ; ++i) {
      floors[i] =
               new Floor(
                        i * Floor::YVALS_PER_FLOOR,
                        i,
                        gfxScaleWidth,
                        i != _nStory-1,
                        i != 0);
   }

   elevators = std::vector<Elevator*> (_nElevator);
   for(unsigned int i=0; i < _nElevator ; ++i ) {
      elevators[i] = new Elevator(0);
   }

   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "finished constructing building @" << this << std::endl;
      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer( dbgSS ));
   }
}

Building::~Building() {
   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "destroying building @" << this << std::endl;
      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer( dbgSS ));
   }

   for(std::vector<Floor*>::iterator iter = floors.begin();
            iter != floors.end();
   ) {
      Floor* currentFloor = *iter;
      iter = floors.erase(iter++);
      delete currentFloor;
   }

   for(std::vector<Elevator*>::iterator iter = elevators.begin();
            iter != elevators.end();
   ) {
      Elevator* currentElevator = *iter;
      iter = elevators.erase(iter++);
      delete currentElevator;
   }
}

/* public methods inherited from SimulationTerminal */
void Building::init() {
   std::for_each(
            floors.begin(),
            floors.end(),
            [] (Floor * thisFloor ) {
      thisFloor -> init();
   });

   std::for_each(
            elevators.begin(),
            elevators.end(),
            [] (Elevator * thisElevator ) {
      thisElevator -> init();
   });
}

void Building::render() {
   glLoadIdentity();
   glTranslatef(2.0f, -2.0f, -1.2f * elevators.size());

   /* TODO: move these values into Building as members */
   static const GLfloat amb[4] = {0.1f, 0.1f, 0.1f, 1.0f};
   static const GLfloat dif[4] = {0.5f, 0.5f, 0.5f, 1.0f};
   static const GLfloat spe[4] = {0.2f, 0.2f, 0.2f, 1.0f};
   static const GLfloat shi = 0.5f;
   static const GLfloat emi[4] = {0.0f, 0.0f, 0.0f, 1.0f};

   glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
   glMaterialfv(GL_FRONT, GL_SPECULAR, spe);
   glMaterialf(GL_FRONT, GL_SHININESS, shi);
   glMaterialfv(GL_FRONT, GL_EMISSION, emi);

   /* adding waiting queue */

   /* Left wall */
   glPushMatrix();
   glTranslatef(
            -gfxScaleWidth - cRenderObjs::GFX_FLOOR_QUEUE_SCALE_WIDTH*2,
            gfxScaleHeight, 0.f);
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
   glTranslatef(
            0 - cRenderObjs::GFX_FLOOR_QUEUE_SCALE_WIDTH,
            gfxScaleHeight, -2.0f);
   glScalef(
            gfxScaleWidth + cRenderObjs::GFX_FLOOR_QUEUE_SCALE_WIDTH,
            gfxScaleHeight, 0.1f);
   glCallList(cRenderObjs::OBJ_CUBE);
   glPopMatrix();

   /* Top wall */
   glPushMatrix();
   glTranslatef(
            0.0f - cRenderObjs::GFX_FLOOR_QUEUE_SCALE_WIDTH,
            gfxScaleHeight*2,
            0.0f);
   glScalef(
            gfxScaleWidth +
            cRenderObjs::GFX_FLOOR_QUEUE_SCALE_WIDTH,
            0.1f, 2.0f);
   glCallList(cRenderObjs::OBJ_CUBE);
   glPopMatrix();

   /* Shafts */
   for(int i=0; i<getMaxElev(); i++)
   {
      glPushMatrix();
      glTranslatef(-gfxScaleWidth + gfxEachElevWidth * i,
               gfxScaleHeight, -0.65f);
      glScalef(0.1f, gfxScaleHeight, .8f);
      glCallList(cRenderObjs::OBJ_CUBE);
      glPopMatrix();
   }


   /* Draw each floor */
   for(unsigned int i=0; i < floors.size(); i++) {
      glPushMatrix();
      glTranslatef(
               0.0f - cRenderObjs::GFX_FLOOR_QUEUE_SCALE_WIDTH,
               gfxEachFloorHeight * i, 0.f);

      floors[i]->render();

      glPopMatrix();
   }

   /* Draw each elevator */
   for(unsigned int i=0; i < elevators.size(); i++) {
      glPushMatrix();
      glTranslatef(
               -gfxScaleWidth + cRenderObjs::ELEV_GAP_WIDTH +
               gfxEachElevWidth * i,
               /* this is in the logical coordinate system,
                * so we divide it by YVALS_PER_FLOOR */
               1.0f +
               (GLfloat)elevators[i]->getYVal() /
               Floor::YVALS_PER_FLOOR *
               gfxEachFloorHeight,
               0.0f);

      /*
       * elev height is on interval
       * [1.0f, 1.0f + (m_nElevator - 1) * gfxEachFloorHeight]
       */

      elevators[i]->render();
      glPopMatrix();
   }

   /* Render land */
   glPushMatrix();
   glTranslatef(-cRenderObjs::GFX_FLOOR_QUEUE_SCALE_WIDTH, 0.f, 0.f);
   glScalef(4.0f + (elevators.size() * 2.0f), 0.0f, 10.0f);
   glCallList(cRenderObjs::OBJ_PLANE);
   glPopMatrix();
}

void Building::update() {
   std::for_each(
            floors.begin(),
            floors.end(),
            [] (Floor* thisFloor) { thisFloor -> update(); });

   std::for_each(
            elevators.begin(),
            elevators.end(),
            [] (Elevator* thisElevator ) { thisElevator -> update(); });

   distributePeople();
}

void Building::updateTuple() {
   if( pythonRepr != NULL ) {
      freeTuple();
   }

   PyObject* floorsTuple = createTupleFromMember(floors);
   PyObject* elevatorsTuple = createTupleFromMember(elevators);
   pythonRepr = Py_BuildValue("(OO)", floorsTuple, elevatorsTuple);
   
   if( pythonRepr == NULL || PyErr_Occurred() ) {
      PyErr_Print();
   }
}

void Building::freeTuple() {

}

void Building::distributePeople() {
   /* roll the dice to see if we'll be adding a person */
   if( rand() % invPersonArriveProb == 0 ) {
      /* pick a random source floor */
      const int sourceFloor = rand() % floors.size();
      int destFloor;

      /* pick a different random dest floor */
      while( (destFloor = rand() % floors.size()) == sourceFloor );

      /* allocate a new person off the heap */
      Person* newPerson = new Person(sourceFloor, destFloor);

      /* add the new randomly generated occupant */
      floors[sourceFloor]->addPerson(newPerson);
   }
}

int Building::getMaxElevHeight() const {
   return ((floors.size() - 1) * Floor::YVALS_PER_FLOOR);
}

int Building::getMinElevHeight() const {
   return 0;
}

} /* namepsace elevatorSim */
