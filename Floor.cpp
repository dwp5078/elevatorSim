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
#include "Floor.hpp"
#include "cRenderObjs.hpp"
#include "Building.hpp"
#include "SimulationState.hpp"

#include <GL/glut.h>
#include <iostream>
#include <sstream>

namespace elevatorSim {

/* public static constants */
const int Floor::YVALS_PER_FLOOR = 5000;

/* constructors */
Floor::Floor(
   int _yVal,
   bool _hasUpperFloor,
   bool _hasLowerFloor) : 
      Location(_yVal), 
      hasUpperFloor(_hasUpperFloor),
      hasLowerFloor(_hasLowerFloor) {

      signalingUp = false;
      signalingDown = false;

      if(isDebugBuild()) {
         std::stringstream dbgSS;
         dbgSS << "constructed floor @" << this << std::endl;
         LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer(dbgSS) );
      }
}

Floor::~Floor() {
   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "destructing floor @" << this << std::endl;
      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer(dbgSS) );
   }
}

void Floor::init() {

}

void Floor::render() {
   GLfloat amb[4] = {0.1f, 0.1f, 0.1f, 1.0f};
   GLfloat dif[4] = {0.5f, 0.5f, 0.5f, 1.0f};
   GLfloat spe[4] = {0.2f, 0.2f, 0.2f, 1.0f};
   GLfloat shi = 0.5f;
   GLfloat emi[4] = {0.0f, 0.0f, 0.0f, 1.0f};

   glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
   glMaterialfv(GL_FRONT, GL_SPECULAR, spe);
   glMaterialf(GL_FRONT, GL_SHININESS, shi);
   glMaterialfv(GL_FRONT, GL_EMISSION, emi);

   glCallList(cRenderObjs::OBJ_CUBE);
}

void Floor::update() {

}

} /* namespace elevatorSim */
