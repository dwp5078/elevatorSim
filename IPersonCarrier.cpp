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
#include "Elevator.hpp"
#include "Logger.hpp"

namespace elevatorSim {

std::unordered_map<Person*, IPersonCarrier*>*
IPersonCarrier::containerCache = NULL;

IPersonCarrier::~IPersonCarrier() {
   /* print debug info */
   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "with IPersonCarrier @ " << this
               << " with " << people.size()
               << " people. destructing them...\n";

      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer( dbgSS ));
   }

   for(std::unordered_set<Person*>::iterator iter = people.begin();
            iter != people.end();
   ) {
      Person* currentMutablePerson = *iter;
      iter = people.erase(iter++);
      delete currentMutablePerson;
   }

   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "destructing in IPersonCarrier @ " << this
               << " complete" << std::endl;

      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer( dbgSS ));
   }
}

std::unordered_map<Person*, IPersonCarrier*>*
IPersonCarrier::acquireContainerCache() {
   return (containerCache == NULL) ?
            (containerCache =
                     new std::unordered_map<Person*, IPersonCarrier*>()) :
                     (containerCache);
}

inline void IPersonCarrier::invalidateCCEntry( Person * const cp ) {
   (void) cp;
}

inline void IPersonCarrier::updateCCEntry(
         Person * const cp,
         IPersonCarrier* icp ) {
   (void) cp;
   (void) icp;
}

inline Person* IPersonCarrier::checkContainerCache( Person * const cp ) {
   (void)cp;

   return NULL;
}

void IPersonCarrier::cleanContainerCache() {
   if(containerCache != NULL ) {
      delete containerCache;
   }
}

} /* namespace ElevatorSim */
