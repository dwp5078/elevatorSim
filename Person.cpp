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
#include "Person.hpp"
#include "Location.hpp"
#include "Logger.hpp"
#include "SimulationState.hpp"

#include <algorithm>

namespace elevatorSim {

/* constructors */
Person::Person(
   Location startLoc,
   Location dest,
   enum PRIORITY p) {
      start = startLoc;
      destination = dest;
      priority = p;

      /* print debug info */
      if(isDebugBuild()) {
         std::stringstream dbgSS;
         dbgSS << "with person @ " << this
            << " start " << start.getYVal()
            << ", dest " << destination.getYVal()
            << ", pri = " << (int)priority << std::endl;

         LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer( dbgSS ));
      }
}

Person::Person( Person&& p ) {
   /* TODO: stub */
   (void) p;
}

Person::~Person() {
   /* print debug info */
   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "destructing person @ " << this << std::endl;
      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer( dbgSS ));
   }
}

void Person::init() {
   /* TODO */
}

void Person::render() {
   /* TODO */
}

void Person::update() {
   /* Then check to see if we've reached a movement condition. If so,
    * either move from floor to elevator or from elevator to floor.
    *
    * NOTE: see line 257 of Elevator::update(), which deletes person
    * instances when they reach their destination floor. This may
    * need to be refactored.
    */

   IPersonCarrier* container = locateContainer();

   Floor* floorContainer = dynamic_cast<Floor*> ( container );
   Elevator* elevatorContainer = dynamic_cast<Elevator*> ( container );

   /* ensure that RTTI is working correctly */
   assert( (floorContainer == NULL) ^ (elevatorContainer == NULL) );

   /* If we're in a floor... */
   if( floorContainer != NULL ) {

      /* TODO: check to see if an elevator has arrived, and
       * get on if it has */


   /* if we're in an elevator... */
   } else if( elevatorContainer != NULL ) {

      /* TODO: check to see if we've arrived at our destination floor,
       * and get off if we have */
   }

   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "updating person @ " << this
         << " with container @ " << container << std::endl;
      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer( dbgSS ));
   }
}

IPersonCarrier* Person::locateContainer() const {
   /* find parent by searching over all floors and elevators,
    * this operation's worst case runs in linear time, O(E+F),
    * where E is number of elevators and F is number of Floors.
    */
   Building& building = SimulationState::acquire().getBuilding();

   Floor** floors = building.getFloors();
   Elevator** elevators = building.getElevators();
   IPersonCarrier* container = NULL;

   for( int i = 0;
      container == NULL && i < building.getStories();
      ++i ) {
         if( floors[i]->containsPerson(this) ) {
            container = floors[i];
         }
   }

   for( int i = 0;
      container == NULL && i < building.getMaxElev();
      ++i ) {
         if( elevators[i]->containsPerson(this) ) {
            container = elevators[i];
         }
   }

   assert( container != NULL );
   return container;
}

} /* namespace ElevatorSim */
