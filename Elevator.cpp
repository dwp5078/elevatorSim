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
#include "Elevator.hpp"
#include "ISimulationTerminal.hpp"
#include "IPersonCarrier.hpp"
#include "Building.hpp"
#include "Floor.hpp"
#include "Logger.hpp"
#include "cTimeManager.hpp"

#include <vector>
#include <set>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <cassert>
#include <ctime>
#include <cmath>

namespace elevatorSim {

   const int Elevator::DEFAULT_MAX_VEL = 120;
   const int Elevator::DEFAULT_MAX_ACCEL = 4;
   const int Elevator::DEFAULT_MAX_OCCUPANTS = 12;
   const int Elevator::DEFAULT_MAX_ANI_COUNTER = 100;
   const int Elevator::DEFAULT_DOOR_CLOSE_DELAY = 70;

   Elevator::Elevator(
      int _yVal,
      const int _maxVel,
      const int _maxAccel,
      const int _maxOccupants) :
      maxVel(_maxVel),
      maxAccel(_maxAccel),
      maxOccupants(_maxOccupants),

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

         init();
         
         /* FOR DEBUG */
         scheduledFloors.push_back( 1 );

         if(isDebugBuild()) {
            std::stringstream dbgSS;
            dbgSS << "constructed elevator @" << this << std::endl;
            LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer( dbgSS ));
         }
   }

   void Elevator::scheduleAccelsToFloor( const int srcFloor, const int destfloor ) {
      SimulationState& simState = SimulationState::acquire();

      assert(destfloor >= 0 &&
         destfloor < simState.getBuilding().getStories() &&
         srcFloor == (yVal / Floor::YVALS_PER_FLOOR));

      /* height of the target floor in yVals */
      int targetFloorHeight = destfloor *  Floor::YVALS_PER_FLOOR;
      int thisFloorHeight = srcFloor *  Floor::YVALS_PER_FLOOR;

      /* the distance traveled at the maximum speed */
      int maxVelTimeInterval =
         (abs(yVal - targetFloorHeight) - 2 * stoppingDistance) / maxVel;

      /* retrieve current logic clock */
      const int currentTime =
         SimulationState::acquire().getTime();

      /* ensure that the total distance scheduled to be traveled is exactly
      * equal to the different in heights between current and distination */
      assert(maxVelTimeInterval * maxVel + 2 * stoppingDistance ==
         abs(targetFloorHeight - thisFloorHeight));

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

   Elevator::~Elevator() {
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
      scheduledFloors.push_back(floor);
   }

   void Elevator::init() {
      currentAccel = 0;
      currentVel = 0;
      closeDoorTimer = -1;
      currentVel = 0;
      currentAccel = 0;

      peopleOffAnimation = false;
      peopleOnAnimation = false;
      peopleOnOffPosX = 0.f;
      peopleOnCounter = 0;
      peopleOffCounter = 0;

      scheduledFloors.clear();
      scheduledAccels.clear();
   }

   void Elevator::render() {
      glPushMatrix();
      glTranslatef(0.f, 0.1f, 1.0f);
      glCallList(cRenderObjs::OBJ_ELEVATOR);
      cRenderObjs::renderOccupants(numPeopleContained(), maxOccupants, false);

      /*  People riding and off elevator animation */
      if(peopleOffAnimation) {
         peopleOffCounter++;

         glPushMatrix();
         glTranslatef(-0.015f * peopleOffCounter, 0.0f, 0.0f);
         cRenderObjs::renderOccupants(1, 30, false);
         glPopMatrix();

         if(peopleOffCounter >= DEFAULT_MAX_ANI_COUNTER)  {
            peopleOffAnimation = false;
            peopleOffCounter = 0;
         }
      }
      else if(!peopleOffAnimation && peopleOnAnimation)   {
         peopleOnCounter--;

         glPushMatrix();
         glTranslatef(-0.015f * peopleOnCounter, 0.0f, 0.0f);
         cRenderObjs::renderOccupants(1, 30, false);
         glPopMatrix();

         if(peopleOnCounter <= 0)  {
            peopleOnAnimation = false;
            peopleOnCounter = DEFAULT_MAX_ANI_COUNTER;
         }
      }

      glColor3f(0.f, 1.0f, 0.f);

      std::stringstream totalSS;
        
      totalSS << numPeopleContained() << " / " << maxOccupants;
      std::string totalCount = totalSS.str();
      cRenderObjs::drawBitmapText((char*)totalCount.c_str(), -0.7f, 1.2f, 1.f);

      glPopMatrix();
   }

   void Elevator::update() {
      const int minElevHeight =
         SimulationState::acquire().getBuilding().getMinElevHeight();
      const int maxElevHeight =
         SimulationState::acquire().getBuilding().getMaxElevHeight();
      const int numFloors =
         SimulationState::acquire().getBuilding().getStories();
      const int currentTime =
         SimulationState::acquire().getTime();

      /* ensure that height and velocity and acceleration are within legal ranges */
      assert( minElevHeight <= yVal && yVal <= maxElevHeight );
      assert( -maxVel <= currentVel && currentVel <= maxVel );
      assert(
         currentAccel == -maxAccel ||
         currentAccel == maxAccel ||
         currentAccel == 0 );

      /* are we on stopped on a floor with another floor scheduled? */
      if( currentVel == 0 &&
         yVal % Floor::YVALS_PER_FLOOR == 0 &&
         scheduledFloors.size() > 0 ) {

            /* is the close door timer unset ? */
            if(closeDoorTimer == -1) {
               /* use this tick to calculate the logic tick offset for opening the door */
               closeDoorTimer = currentTime + DEFAULT_DOOR_CLOSE_DELAY;
            } else if (closeDoorTimer < currentTime) {
               /* otherwise if the door has closed... */
               closeDoorTimer = -1;

               /* stop drawing any people that are dangling */
               peopleGetOnOffAnimationOff();

               /* comute this floor and retrieve the next floor */
               const int thisFloor = (yVal / Floor::YVALS_PER_FLOOR);
               const int nextFloor = scheduledFloors.back();

               /* remove this floor from the scheduled floors queue */
               scheduledFloors.pop_back();

               /* if it's different from this floor, schedule the accelerations */
               if( thisFloor != nextFloor ) {
                  scheduleAccelsToFloor(thisFloor, nextFloor);
               }

               /* FOR DEBUG: schedule a new random dest upon arriving at a floor
               * if there are no stops after this */
               if( scheduledFloors.size() == 0 ) {
                  scheduledFloors.push_back(rand() % numFloors);

                  /* call into python and process the user's elevator script 
                   * (work in progress) */
                  SimulationState::acquire().runUserScriptUnsafe();
               }
            }
      }

      /* is there a scheduled acceleration pending? */
      if( scheduledAccels.size() ) {
         std::pair<int, int> nextScheduledAccel = scheduledAccels.back();

         /* is the next scheduled acceleration now? */
         if( nextScheduledAccel.first == currentTime ) {

            /* remove it from the vector */
            scheduledAccels.pop_back();

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

      /* update occupants */
      for(std::unordered_set<Person*>::iterator iter = people.begin();
         iter != people.end();
         ) {
            /* obtain a pointer to the current person by using iterator */
            Person* currentMutablePerson = *iter;

            /* copy construct an iterator from the current one */
            std::unordered_set<Person*>::iterator nextPosition = std::unordered_set<Person*>::iterator(iter);

            /* increment iterator position, to save the next position */
            ++nextPosition;

            currentMutablePerson -> update();

            /* the current iterator could've been invalidated by a person moving itself, 
            * so intead of iter++ we just overwrite with the saved position */
            iter = nextPosition;     
      }

      /* ensure that height and velocity and acceleration are within legal ranges */
      assert( minElevHeight <= yVal && yVal <= maxElevHeight );
      assert( -maxVel <= currentVel && currentVel <= maxVel );
      assert(
         currentAccel == -maxAccel ||
         currentAccel == maxAccel ||
         currentAccel == 0  );
   }

   int Elevator::getCurrentFloor()   {
      return yVal / Floor::YVALS_PER_FLOOR;
   }

} /* namespace elevatorSim */
