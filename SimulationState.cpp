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
#include "SimulationState.hpp"
#include "cTimeManager.hpp"
#include "cKeyManager.hpp"
#include "cRenderObjs.hpp"
#include "cCameraManager.hpp"
#include "Building.hpp"

#include <set>
#include <functional>
#include <cassert>

namespace elevatorSim   {

SimulationState* SimulationState::simulationState = NULL;

SimulationState::SimulationState() : cState(SimulationState::SIMULATION_STARTING) {
   stateObjects.insert((timeManager = new cTimeManager()));
   stateObjects.insert((keyManager = new cKeyManager()));
   stateObjects.insert((cameraManager = new cCameraManager()));
   stateObjects.insert((building = new Building()));

   renderObjs = new cRenderObjs();
}

SimulationState::~SimulationState() {
   delete building;

   delete cameraManager;
   delete renderObjs;
   delete keyManager;
   delete timeManager;
}

SimulationState& SimulationState::acquire() {
   if(simulationState == NULL) {
      simulationState = new SimulationState();
   }

   return *simulationState;
}

void SimulationState::release() {
   assert(simulationState != NULL);
   delete simulationState;
   simulationState = NULL;
}

void SimulationState::init() {
   cState = SIMULATION_RUNNING;

   /* TODO */
}

void SimulationState::update() {

   /* lambdas are sexy */

   std::for_each(
      stateObjects.begin(),
      stateObjects.end(),
      [] (IStateObject * stateObj) { 
   
         /* TODO: acquire a lock and call update */
         (void) stateObj;

   });
}

} /* namespace elevatorSim */
