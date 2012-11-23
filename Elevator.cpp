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
#include "Logger.hpp"
#include "cTimeManager.hpp"

#include <boost/math/special_functions/round.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>
#include <ctime>
#include <cmath>

namespace elevatorSim {

const int Elevator::DEFAULT_MAX_VEL = 30;
const int Elevator::DEFAULT_MAX_ACCEL = 3;
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

   /* distance required to stop when traveling
    * at maximum speed and then experiencing
    * negative maximum acceleration in the opposing
    * the direction of motion */
   stoppingDistance(boost::math::iround(
      maxVel * accelTimeInterval -
      maxAccel * (accelTimeInterval * accelTimeInterval ) / 2.0f)),

   /* deceleration time from max speed to 0,
    * or acceleration time from 0 to max speed */
   accelTimeInterval(boost::math::iround((float)maxVel / maxAccel)) {

   yVal = _yVal;
   currentVel = 0;
   currentAccel = maxAccel; /* NOTE: THIS IS FOR TESTING PURPOSES */
   floorsSignaled = new bool[numFloors];

   ///////////////////Test - Soohoon
   destFloor = -1;

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

   LOG_INFO( Logger::SUB_ELEVATOR_LOGIC, "in canStopAtNextFloor()..." );

   if(currentAccel == 0) {
      return false;
   }

   /* compute next floor by truncating current and adding or subtracting
    * based on the current velocity */
   int nextFloor = int(yVal / Floor::YVALS_PER_FLOOR)
      + (currentVel > 0) ? (1) : (-1);

   int nextFloorHeight = nextFloor * Floor::YVALS_PER_FLOOR;

   /* compute distance to next floor
    * based on the current velocity */
   int nextFloorDistance = ( currentVel > 0 ) ?
      ( nextFloorHeight - yVal ) :
      ( yVal - nextFloorHeight );

   /* check if there's ample distance to stop */
   return nextFloorDistance <= stoppingDistance;
}

void Elevator::goToFloor(int floor) {
   assert(floor >= 0 && floor < numFloors);

   /* height of the target floor in yVals */
   int nextFloorHeight = floor *  Floor::YVALS_PER_FLOOR;

   /* we're already there, so do nothing */
   if(yVal == nextFloorHeight) {
      return;
   }

   /* the distance traveled at the maximum speed */
   int maxVelTimeInterval = boost::math::iround(
      ((float)(abs(yVal - nextFloorHeight) - stoppingDistance) / maxVel));

   scheduledAccels.push_back(
      std::pair<int, int> (accelTimeInterval,
      (yVal < nextFloorHeight) ? ( maxAccel ) : ( -maxAccel )));

   scheduledAccels.push_back(
       std::pair<int, int> (maxVelTimeInterval, 0) );

   scheduledAccels.push_back(
       std::pair<int, int> (accelTimeInterval,
       (yVal < nextFloorHeight) ? ( -maxAccel ) : ( maxAccel )));
}

void Elevator::init() {
   /* TODO */
}

void Elevator::render() {
   glCallList(cRenderObjs::OBJ_ELEVATOR);
   glCallList(cRenderObjs::OBJ_HUMAN);
}

void Elevator::update() {
   /* ensure that accel is either -maxAccel, +maxAccel, or 0 */
   assert(
      currentAccel == -maxAccel ||
      currentAccel == maxAccel ||
      currentAccel == 0  );

   const int minElevHeight = SimulationState::acquire().getBuilding().getMinElevHeight();
   const int maxElevHeight = SimulationState::acquire().getBuilding().getMaxElevHeight();

   ///////////////////Test Block Start-  Soohoon
   generateRandomDest();
   int finalPos = destFloor * Floor::YVALS_PER_FLOOR;
   int diff = abs(finalPos - yVal);

   if(diff <= Floor::YVALS_PER_FLOOR)  {  //decelerate
   ///////////////////Test Block End

      if(currentAccel > 0) {
         /* replace current vel with current vel + accel,
          * unless it's greater than the maximum vel */
         currentVel = ( currentVel + currentAccel > maxVel ) ?
            ( maxVel ) : ( currentVel + currentAccel );
         /* otherwise if current accel is negative... */
      } else if(currentAccel < 0) {
         /* replace current vel with current vel + accel,
          * unless it's less than the minimum vel */
         currentVel = ( currentVel + currentAccel < -maxVel ) ?
            ( -maxVel ) : ( currentVel + currentAccel );
      }

      /* if current vel is positive */
      if(currentVel > 0) {
         /* replace current yVal with yVal plus current vel,
          * unless it's greater than the maximum yVal */
         yVal = (yVal + currentVel < finalPos) ?
            ( yVal + currentVel ) : ( finalPos );
         /* otherwise if current vel is negative */
      } else if (currentVel < 0) {
         /* replace current yVal with yVal + current vel,
          * unless it's less than the minimum yVal */
         yVal = (yVal + currentVel > finalPos) ?
            ( yVal + currentVel ) : ( finalPos );
      }

   //////////////////Test Block Start
      /*if(yVal == finalPos)  {
         if(waitingTime == 0) {
          waitingTime = cTimeManager::worldTime();
       }
         if(waitingTime + 1500 <= cTimeManager::worldTime() )   {
          waitingTime = 0;
            destFloor = -1;
            generateRandomDest();

         }
      }*/
     if(yVal == finalPos) {
         static int temp = 0;
         temp++;
         if(temp == 200) {
            temp = 0;
            destFloor = -1;
            generateRandomDest();

         }
      }
   } else  {
   ///////////////////Test Block End


   /* if current accel is positive... */
   if(currentAccel > 0) {
      /* replace current vel with current vel + accel,
       * unless it's greater than the maximum vel */
      currentVel = (currentVel + currentAccel < maxVel ) ?
         ( currentVel + currentAccel ) : ( maxVel );
   /* otherwise if current accel is negative... */
   } else if(currentAccel < 0) {
      /* replace current vel with current vel + accel,
       * unless it's less than the minimum vel */
      currentVel = (currentVel + currentAccel > -maxVel ) ?
         ( currentVel + currentAccel ) : ( -maxVel );
   }

   /* if current vel is positive */
   if(currentVel > 0) {
      /* replace current yVal with yVal plus current vel
       * unless it's greater than the maximum yVal */
      yVal = (yVal + currentVel < maxElevHeight) ?
         ( yVal + currentVel ) : ( maxElevHeight );
   /* otherwise if current vel is negative */
   } else if (currentVel < 0) {
      /* replace current yVal with yVal + current vel
       * unless it's less than the minimum yVal */
      yVal = (yVal + currentVel > minElevHeight) ?
         ( yVal + currentVel ) : ( minElevHeight );
   }

   ////////////////Test Block Start
   }
   ////////////////Test Block End

   assert( minElevHeight <= yVal && yVal <= maxElevHeight );
   assert( -maxVel <= currentVel && currentVel <= maxVel );
}

///////////////////Test Function - Soohoon
void Elevator::generateRandomDest()
{
   if(destFloor != -1)  return;

   destFloor = rand() % SimulationState::acquire().getBuilding().getStories();

   if(yVal < destFloor * Floor::YVALS_PER_FLOOR) {
      currentAccel = maxAccel;
   } else {
      currentAccel = -maxAccel;
   }

   currentVel = 0;
}

} /* namespace elevatorSim */
