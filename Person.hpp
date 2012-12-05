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

#ifndef _PERSON_H
#define _PERSON_H

#include "Location.hpp"
#include "Building.hpp"
#include "Elevator.hpp"
#include "ISimulationTerminal.hpp"

namespace elevatorSim {

class Building;
class Elevator;

class Person : public ISimulationTerminal {
   /* friends */
   friend class Building;
   friend class Floor;
   friend class Elevator;

   /* private static constants */
   enum PRIORITY {
      UNKNOWN,
      NORMAL,
      HIGH,
      EMERGENCY
   };

   /* private static methods */

   /* private instance members */
   Location current;
   Location destination;
   enum PRIORITY priority;

   /* private methods */

   /* constructors */
   Person(
      Location startLoc,
      Location dest,
      enum PRIORITY p=UNKNOWN);

public:

   /* public static constants */

   /* public instance members */

   ~Person();

   /* public const methods */
   Location getDestination() const {
      return destination;
   }

   enum PRIORITY getPriority() const {
      return priority;
   }

   /* public methods inherited from ISimulationTerminal*/
   void init();
   void render();
   void update();

};

} /* namespace elevatorSim */

#endif /* _PERSON_H */
