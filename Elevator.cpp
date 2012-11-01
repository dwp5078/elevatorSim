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

#include <vector>

#include "Elevator.hpp"

namespace elevatorSim {

Elevator::Elevator() : maxVel(0), maxAccel(0), maxOccupants(0) {

}

Elevator::Elevator(const int _maxVel, const int _maxAccel, const int _maxOccupants)
: maxVel(_maxVel), maxAccel(_maxAccel), maxOccupants(_maxOccupants) {
   currentVel = 0;
   currentAccel = 0;

   /* TODO */
}

Elevator::Elevator(const Elevator & copy) : 
   Location(),
   maxVel(copy.maxVel), 
   maxAccel(copy.maxAccel), 
   maxOccupants(copy.maxOccupants) {
      currentVel = copy.currentVel;
      currentAccel = copy.currentAccel;
      occupants.assign(copy.occupants.begin(), copy.occupants.end());
}

void Elevator::init()
{
   /* TODO */
}

void Elevator::render() {
   glCallList(cRenderObjs::OBJ_ELEVATOR);
}

void Elevator::update()
{
   /* TODO */
}

}

