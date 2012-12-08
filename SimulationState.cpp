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
#include "SimulationState.hpp"
#include "cTimeManager.hpp"
#include "cKeyManager.hpp"
#include "cRenderObjs.hpp"
#include "cCameraManager.hpp"
#include "Building.hpp"
#include "Logger.hpp"

#include <boost/thread/mutex.hpp>
#include <set>
#include <functional>
#include <cassert>
#include <sstream>
#include <string>
#include <fstream>

namespace elevatorSim   {

SimulationState* SimulationState::simulationState = NULL;

SimulationState::SimulationState() : cState(SimulationState::SIMULATION_STARTING) {
   stateObjects.insert((timeManager = new cTimeManager()));
   stateObjects.insert((keyManager = new cKeyManager()));
   stateObjects.insert((cameraManager = new cCameraManager()));
   stateObjects.insert((building = new Building()));

   renderObjs = new cRenderObjs();
   userScript = NULL;
}

SimulationState::~SimulationState() {
   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "starting to destroy simulation state @ " << this << std::endl;
      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer(dbgSS) );
   }

   init();

   delete building;

   delete cameraManager;
   delete renderObjs;
   delete keyManager;
   delete timeManager;

   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "finished destroying simulation state @ " << this << std::endl;
      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer(dbgSS) );
   }
}

SimulationState& SimulationState::acquire() {
   if(simulationState == NULL) {
      simulationState = new SimulationState();
      simulationState -> init();
   }

   return *simulationState;
}

void SimulationState::release() {
   assert(simulationState != NULL);
   delete simulationState;
   simulationState = NULL;
}

void SimulationState::init() {
   cState = SIMULATION_STARTING;
   logicTicks = 0;

   if( userScript != NULL) {
      Py_DECREF(userScript);
      userScript = NULL;
   }
}

void SimulationState::update() {
   bigAssStateMutex.lock();

   if( cState == SIMULATION_RUNNING ) {
      std::for_each(
         stateObjects.begin(),
         stateObjects.end(),
         [] (IStateObject * stateObj) {
            stateObj -> update();
      });

      ++logicTicks;
   }

   bigAssStateMutex.unlock();
}

void SimulationState::start(
   int numElevators,
   int numFloors,
   int randomSeed,
   const std::string& pyAiPath ) {
      assert(userScript == NULL);

      bigAssStateMutex.lock();

      /* cRenderObjs */

      init();

      /* load and compile python */
      if( loadPythonScript( pyAiPath ) ) {

         timeManager->init();
         keyManager->init();
         cameraManager->init();

         stateObjects.erase(building);
         delete building;
         building = new Building(numFloors, numElevators);
         stateObjects.insert(building);

         cState = SIMULATION_RUNNING;
      }

      bigAssStateMutex.unlock();
}

bool SimulationState::togglePause() {
   bool ret = false;
   bigAssStateMutex.lock();

   if( cState != SIMULATION_RUNNING && cState != SIMULATION_PAUSED ) {
      ret = false;      
   } else {

      if( cState == SIMULATION_PAUSED ) {
         ret = false;
         cState = SIMULATION_RUNNING;
      } else {
         ret = true;
         cState = SIMULATION_PAUSED;
      }
   }

   bigAssStateMutex.unlock();
   return ret;
}

void SimulationState::stop() {
   bigAssStateMutex.lock();
   init();
   cState = SIMULATION_READY;

   /* TODO: post-process simulation data */

   bigAssStateMutex.unlock();
}

bool SimulationState::loadPythonScript( const std::string& pyAiPath ) {
   std::string pyBuffer;

   std::ifstream pyScriptFile( pyAiPath.c_str(), std::ifstream::in );
   if (pyScriptFile.is_open()) {
      int lineCount = 0;
      while ( pyScriptFile.good() ) {
         std::string lineBuffer;
         getline (pyScriptFile, lineBuffer);
         pyBuffer += lineBuffer;
         ++lineCount;
      }

      pyScriptFile.close();
      (void) lineCount;

      /* see http://docs.python.org/3/c-api/veryhigh.html#Py_CompileStringExFlags */

      /* compile the string into an object */
      userScript =
         Py_CompileString(
            pyBuffer.c_str(),
            pyAiPath.c_str(),
            Py_file_input);

      if( userScript == NULL ) {
         PyErr_Print();
         return false;
      }

   } else {
      LOG_ERROR( Logger::SUB_GENERAL, "couldn't open script file");
      return false;
   }

   return true;
}

} /* namespace elevatorSim */
