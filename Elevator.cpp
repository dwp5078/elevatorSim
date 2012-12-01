/*
 * Copyright (c) 2012, Joseph Max DeLiso, Daniel Gilbert
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
#include "Elevator.hpp"
#include "ISimulationTerminal.hpp"
#include "Building.hpp"
#include "Floor.hpp"
#include "Logger.hpp"
#include "cTimeManager.hpp"

#include <boost/math/special_functions/round.hpp>
#include <boost/static_assert.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>
#include <ctime>
#include <cmath>

namespace elevatorSim {

const int Elevator::DEFAULT_MAX_VEL = 120;
const int Elevator::DEFAULT_MAX_ACCEL = 4;
const int Elevator::DEFAULT_MAX_OCCUPANTS = 12;

Elevator::Elevator(
   int _yVal,
   const int _numFloors,
   const int _maxVel,
   const int _maxAccel,
   const int _maxOccupants) :
   maxVel(_maxVel),
   maxAccel(_maxAccel),
   maxOccupants(_maxOccupants),
   numFloors(_numFloors),

   /* deceleration time from max speed to 0,
    * or acceleration time from 0 to max speed */
   accelTimeInterval(maxVel / maxAccel),

   /* distance required to stop when traveling
    * at maximum speed and then experiencing
    * negative maximum acceleration in the opposing
    * the direction of motion */
   stoppingDistance(
      maxVel * accelTimeInterval -
      maxAccel * (accelTimeInterval * accelTimeInterval ) / 2) {

         /* avoid truncation issues */
         assert(maxVel % maxAccel == 0);

         /* make sure that the elevator can achieve maximum speed both
          * while accelerating and slowing down between one floor */
         assert( 2 * stoppingDistance < Floor::YVALS_PER_FLOOR);

         yVal = _yVal;
         currentVel = 0;
         currentAccel = 0; 
         floorsSignaled = new bool[numFloors];

         if(isDebugBuild()) {
            std::stringstream dbgSS;
            dbgSS << "constructed elevator @" << this << std::endl;
            LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer( dbgSS ));
         }
}

Elevator::~Elevator() {
   delete [] floorsSignaled;

   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "destructing delevator @" << this << std::endl;
      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer( dbgSS ));
   }
}

bool Elevator::canStopAtNextFloor() {
   /*
    * This function checks the acceleration and velocity and position and
    * returns true if the elevator can stop at the floor it's headed towards.
    * If the elevator is not accelerating, the function returns false.
    */

   if(currentAccel == 0) {
      return false;
   }

   /* compute next floor by truncating current and adding or subtracting
    * based on the current velocity */
   int nextFloor = int(yVal / Floor::YVALS_PER_FLOOR) 
      + ((currentVel > 0) ? (1) : (-1));

   int nextFloorHeight = nextFloor * Floor::YVALS_PER_FLOOR;

   /* compute distance to next floor */
   int nextFloorDistance = abs( nextFloorHeight - yVal );

   /* check if there's ample distance to stop */
   bool canStop = nextFloorDistance <= stoppingDistance;

   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "with elevator @ " << this 
        << " and a = " << currentAccel << " v = " << currentVel << " y = " << yVal
        << std::endl << " SD= " << stoppingDistance << " NFD= " << nextFloorDistance
        << " can stop? " << canStop << std::endl;

      LOG_INFO( Logger::SUB_ELEVATOR_LOGIC, sstreamToBuffer( dbgSS ));
   }

   return canStop;
}

void Elevator::goToFloor(int floor) {
   assert(floor >= 0 && floor < numFloors);

   /* height of the target floor in yVals */
   int targetFloorHeight = floor *  Floor::YVALS_PER_FLOOR;
   const int currentTime =
      SimulationState::acquire().getTime();

   /* we're already there, so do nothing */
   if(yVal == targetFloorHeight) {
      return;
   }

   /* the distance traveled at the maximum speed */
   int maxVelTimeInterval = boost::math::iround(
      ((float)(abs(yVal - targetFloorHeight) - 2 * stoppingDistance) / maxVel));

   /* ensure that there are no truncation issues */
   assert(maxVelTimeInterval * maxVel + 2 * stoppingDistance == targetFloorHeight);
      
   /* print debug info */
   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "with elevator @ " << this
         << " time t=" << currentTime
         << " a = " << currentAccel << " v = " << currentVel << " y = " << yVal
         << " and " << scheduledAccels.size() << " scheduled accels. " << std::endl;

      LOG_INFO( Logger::SUB_ELEVATOR_LOGIC, sstreamToBuffer( dbgSS ));
   }

   /* push them onto the back of the vector in reverse order */
   scheduledAccels.push_back(
      std::pair<int, int> ( currentTime + 2 * accelTimeInterval + maxVelTimeInterval, 0 ));

   scheduledAccels.push_back(
      std::pair<int, int> ( currentTime + accelTimeInterval + maxVelTimeInterval, 
      ( yVal < targetFloorHeight) ? ( -maxAccel ) : ( maxAccel )));

   scheduledAccels.push_back(
      std::pair<int, int> ( currentTime + accelTimeInterval, 0 ));

   scheduledAccels.push_back(
      std::pair<int, int> ( currentTime,
      ( yVal < targetFloorHeight) ? ( maxAccel ) : ( -maxAccel )));
}

void Elevator::init() {
   /* TODO */
}

void Elevator::render() {
   glCallList(cRenderObjs::OBJ_ELEVATOR);

   int num = getOccupantSize();
   if(num > 0)
      glCallList(cRenderObjs::OBJ_HUMAN);
}

void Elevator::update() {
   const int minElevHeight = 
      SimulationState::acquire().getBuilding().getMinElevHeight();
   const int maxElevHeight = 
      SimulationState::acquire().getBuilding().getMaxElevHeight();
   const int currentTime =
      SimulationState::acquire().getTime();

   /* ensure that height and velocity and acceleration are within legal ranges */
   assert( minElevHeight <= yVal && yVal <= maxElevHeight );
   assert( -maxVel <= currentVel && currentVel <= maxVel );
   assert(
      currentAccel == -maxAccel ||
      currentAccel == maxAccel ||
      currentAccel == 0 );

   if( currentTime == 100 ) {
      goToFloor(21);
   }
      
   /* is there a scheduled acceleration pending? */
   if( scheduledAccels.size() ) {
      std::pair<int, int> nextScheduledAccel = scheduledAccels.back();

      /* is the next scheduled acceleration now? */
      if( nextScheduledAccel.first == currentTime ) {

         /* remove it from the vector */
         scheduledAccels.pop_back();

         /* print debug info */
         if(isDebugBuild()) {
            std::stringstream dbgSS;
            dbgSS << "with elevator @ " << this 
               << " pulling scheduled accel off of queue at t = " << currentTime
               << ", a = " << currentAccel << ", v = " << currentVel << ", y = " << yVal
               << " and now " << scheduledAccels.size() 
               << " scheduled accels. NEW ACCEL: " << nextScheduledAccel.second << std::endl;

            LOG_INFO( Logger::SUB_ELEVATOR_LOGIC, sstreamToBuffer( dbgSS ));
         }

         /* and overwrite the current accel */
         currentAccel = nextScheduledAccel.second;
      }
   } 

   /* update current velocity */
   currentVel += currentAccel;
   currentVel = ( currentVel > maxVel ) ? ( maxVel ) : ( currentVel );
   currentVel = ( currentVel < -maxVel ) ? ( -maxVel ) : ( currentVel ); 

   /* update current position */
   yVal += currentVel;
   yVal = (yVal > maxElevHeight ) ? ( maxElevHeight ) : ( yVal );
   yVal = (yVal < minElevHeight ) ? ( minElevHeight ) : ( yVal );

   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "elevator @" << this 
         << " yVal " << yVal << " v " << currentVel 
         << " a " << currentAccel << std::endl;
      LOG_INFO( Logger::SUB_ELEVATOR_LOGIC, sstreamToBuffer( dbgSS ));
   } 
 
   /* ensure that height and velocity and acceleration are within legal ranges */
   assert( minElevHeight <= yVal && yVal <= maxElevHeight );
   assert( -maxVel <= currentVel && currentVel <= maxVel );
   assert(
      currentAccel == -maxAccel ||
      currentAccel == maxAccel ||
      currentAccel == 0  );
}

void Elevator::pickupOccupants(Floor* floor)
{
   std::vector<Person> *floorOccupants = floor->getOccupants();

   // Take out people from elevator
   if(getOccupantSize() > 0)  {
      std::vector<Person>::iterator p;
      for(p = occupants.begin(); p != occupants.end(); p++) {
         if(p->destination.getYVal() == getCurrentFloor())   {
            occupants.erase(p);
         }
      }
   }

   // Bring people from floor
   std::for_each(
      floorOccupants->begin(),
      floorOccupants->end(),
      [] (Person p) {

   });
   
}

int Elevator::getCurrentFloor()   {
   return yVal / Floor::YVALS_PER_FLOOR;
}

} /* namespace elevatorSim */
