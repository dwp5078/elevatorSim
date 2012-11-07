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

#include "Elevator.hpp"
#include "SimulationTerminal.hpp"
#include "Building.hpp"

#include <boost/math/special_functions/round.hpp>
#include <vector>
#include <iostream>
#include <cassert>
#include <ctime>

namespace elevatorSim {

const int Elevator::DEFAULT_MAX_VEL = 25; //5
const int Elevator::DEFAULT_MAX_ACCEL = 1;  //3
const int Elevator::DEFAULT_MAX_OCCUPANTS = 12;

Elevator::Elevator(
   const Building& _owner,
   int _yVal,
   const int _maxVel,
   const int _maxAccel,
   const int _maxOccupants) : owner(_owner), maxVel(_maxVel), maxAccel(_maxAccel), maxOccupants(_maxOccupants)  {

   yVal = _yVal;
   currentVel = 0;
   currentAccel = maxAccel; /* NOTE: THIS IS FOR TESTING PURPOSES */

   floorsSignaled = new bool[owner.getStories()];

   ///////////////////Test - Soohoon
   destFloor = -1;

   if(isDebugBuild()) {
      std::cout << "constructed elevator with owner building @" << &owner << std::endl;
   }
}

Elevator::~Elevator() {
   delete [] floorsSignaled;

   if(isDebugBuild()) {
      std::cout << "destructing delevator @" << this << std::endl;
   }
}

bool Elevator::canStopAtNextFloor() {
   /*
    * This function checks the acceleration and velocity and position and returns true
    * if the elevator can stop at the floor it's headed towards. If the elevator is not
    * accelerating, the function returns false.
    */

   int nextFloor = 0;        /* next floor */
   int nextfloorHeight = 0;  /* height of next floor */
   /* unused */ /* int floor_elev_distance; */
   /* the distance between the elevator and next floor */
   int acc_time = 0;         /* acceleration time */
   int distance_needed = 0;  /* the distance needed by elevator to stop */

   if(currentAccel < 0) {
      if (currentVel > 0) {  /* if the elevator is going up, and acceleration < 0,the elevator will stop somewhere */

         nextFloor = int(yVal / Floor::YVALS_PER_FLOOR) + 1;
         nextfloorHeight = nextFloor * Floor::YVALS_PER_FLOOR;
         /* unused */ /* floor_elev_distance = nextfloorHeight - yVal; */

         /* calculate how much time the elevator needs to stop, V = V0 + at, V = 0; V0 = VE */
         acc_time = boost::math::iround(-currentVel / currentAccel);

         /* calculate distance needed, X = v * t * a * t^2 / 2 */
         distance_needed = boost::math::iround(currentVel * acc_time + currentAccel * (acc_time * acc_time)/2);

         if (distance_needed <= (nextfloorHeight - yVal)) {
            return true;
         } else {
            return false;
         }
      } else { 
         return false; /* if the elevator is going down or staying still and acceleration < 0, the elevator will NEVER stop */
      }
   } else if(currentAccel > 0) {
      if (currentVel < 0) {     /* if the elevator is going down, and acceleration > 0, the elevator will stop somewhere */
         nextFloor = int(yVal / Floor::YVALS_PER_FLOOR) + 1;
         nextfloorHeight = nextFloor * Floor::YVALS_PER_FLOOR;
         /* unused */ /* floor_elev_distance = nextfloorHeight - Location::yVal; */

         /* calculate how much time the elevator needs to stop, V = V0 + at, V = 0; V0 = VE */
         acc_time = boost::math::iround(-currentVel / currentAccel);

         /* calculate distance needed, X = v * t * a * t^2 / 2 */
         distance_needed = boost::math::iround(currentVel * acc_time + currentAccel * (acc_time * acc_time)/2);

         if (distance_needed <= (nextfloorHeight - yVal)) {
            return true;
         } else {
            return false;
         }
      } else {
         return false;            /*if the elevator is going up or staying still and acceleration > 0,the elevator will NEVER stop */
      }
   } else { /* currentAccel == 0 */
      return false;
   }
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
   assert(currentAccel == -maxAccel || currentAccel == maxAccel || currentAccel == 0  );

   ///////////////////Test Block Start-  Soohoon
   generateRandomDest();
   int finalPos = destFloor * Floor::YVALS_PER_FLOOR;
   int diff = abs(finalPos - yVal);

   if(diff <= Floor::YVALS_PER_FLOOR / 2)  {  //decelerate
   ///////////////////Test Block End

      if(currentAccel > 0) {
         /* replace current vel with current vel + accel, unless it's greater than the maximum vel*/
         currentVel = (currentVel - 1 > 0 ) ? ( currentVel - 1 ) : ( 5 );
         /* otherwise if current accel is negative... */
      } else if(currentAccel < 0) {
         /* replace current vel with current vel + accel, unless it's less than the minimum vel */
         currentVel = (currentVel + 1 < 0 ) ? ( currentVel + 1 ) : ( -5 );
      }

      /* if current vel is positive */
      if(currentVel > 0) {
         /* replace current yVal with yVal plus current vel, unless it's greater than the maximum yVal */
         yVal = (yVal + currentVel < finalPos) ? ( yVal + currentVel ) : ( finalPos );
         /* otherwise if current vel is negative */
      } else if (currentVel < 0) {
         /* replace current yVal with yVal + current vel, unless it's less than the minimum yVal */
         yVal = (yVal + currentVel > finalPos) ? ( yVal + currentVel ) : ( finalPos );
      }

   //////////////////Test Block Start
      if(yVal == finalPos)  {
         static int temp = 0;
         temp++;
         if(temp == 200)   {
            temp = 0;
            destFloor = -1;
            generateRandomDest();

         }
      }
   } else  {
   ///////////////////Test Block End


   /* if current accel is positive... */
   if(currentAccel > 0) {
      /* replace current vel with current vel + accel, unless it's greater than the maximum vel*/
      currentVel = (currentVel + currentAccel < maxVel ) ? ( currentVel + currentAccel ) : ( maxVel );
   /* otherwise if current accel is negative... */
   } else if(currentAccel < 0) {
      /* replace current vel with current vel + accel, unless it's less than the minimum vel */
      currentVel = (currentVel + currentAccel > -maxVel ) ? ( currentVel + currentAccel ) : ( -maxVel );
   }

   /* if current vel is positive */
   if(currentVel > 0) {
      /* replace current yVal with yVal plus current vel, unless it's greater than the maximum yVal */
      yVal = (yVal + currentVel < owner.getMaxElevHeight()) ? ( yVal + currentVel ) : ( owner.getMaxElevHeight() );
   /* otherwise if current vel is negative */
   } else if (currentVel < 0) {
      /* replace current yVal with yVal + current vel, unless it's less than the minimum yVal */
      yVal = (yVal + currentVel > owner.getMinElevHeight()) ? ( yVal + currentVel ) : ( owner.getMinElevHeight() );
   }

   ////////////////Test Block Start
   }
   ////////////////Test Block End

   assert( owner.getMinElevHeight() <= yVal && yVal <= owner.getMaxElevHeight() );
   assert( -maxVel <= currentVel && currentVel <= maxVel );
}

///////////////////Test Function - Soohoon
void Elevator::generateRandomDest()
{
   if(destFloor != -1)  return;

   destFloor = rand() % owner.getStories();

   if(yVal < destFloor * Floor::YVALS_PER_FLOOR) {
      currentAccel = maxAccel;
   } else {
      currentAccel = -maxAccel;
   }

   currentVel = 0;
}

} /* namespace elevatorSim */

