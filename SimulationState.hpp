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

#ifndef _SIMULATION_STATE_H_
#define _SIMULATION_STATE_H_

#include "ElevatorSim.hpp"
#include "cTimeManager.hpp"
#include "cKeyManager.hpp"
#include "cRenderObjs.hpp"
#include "cCameraManager.hpp"
#include "Building.hpp"

#include <cassert>
#include <set>
#include <boost/thread/mutex.hpp>

namespace elevatorSim {

class cTimeManager;
class cKeyManager;
class cRenderObjs;
class cCameraManager;
class Building;

class SimulationState : public IStateObject {
public:

   enum StateKind {
      SIMULATION_STARTING,
      SIMULATION_READY,
      SIMULATION_RUNNING,
      SIMULATION_PAUSED,
      SIMULATION_KILLED
   };

   static SimulationState& acquire();
   static void release();

   inline cKeyManager& getKeyManager() {
      return *keyManager;
   }

   inline cTimeManager& getTimeManager() {
      return *timeManager;
   }

   inline cCameraManager& getCameraManager() {
      return *cameraManager;
   }

   inline Building& getBuilding() {
      return *building;
   }

   void initRenderObjs() {
      renderObjs->init(); /* TODO: refactor this out */
   }

   void init();
   void update();

   void start(
      int numElevators, 
      int numFloors, 
      int randomSeed, 
      const std::string& pyAI );

   /* TODO: something more elaborate and safe here */
   inline void notifyKill() {
      cState = SIMULATION_KILLED;
   }

   enum StateKind getState() {
      StateKind ret;

      if(bigAssStateMutex.try_lock()) {
         ret = cState;
         bigAssStateMutex.unlock();
      } else {
         ret = SIMULATION_STARTING;
      }

      return ret;
   }

   enum StateKind getStateUnsafe() {
      return cState;
   }

   inline void lockBASM() {
      bigAssStateMutex.lock();
   }

   inline void unlockBASM() {
      bigAssStateMutex.unlock();
   }

   inline int getTime() const {
      return logicTicks;
   }

private:
   
   static SimulationState* simulationState;

   std::set<IStateObject*> stateObjects;
   boost::mutex bigAssStateMutex;

   StateKind cState;
   cTimeManager* timeManager;
   cKeyManager* keyManager;
   cRenderObjs* renderObjs;
   cCameraManager* cameraManager;
   int logicTicks;

   Building* building;

   SimulationState();
   ~SimulationState();

};

} /* namespace elevatorSim */

#endif /* _SIMULATION_STATE_H */
