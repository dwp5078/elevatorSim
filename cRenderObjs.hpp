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

#ifndef __CRENDEROBJS_HPP_
#define __CRENDEROBJS_HPP_

#include "ElevatorSim.hpp"

#include <GL/glut.h>

namespace elevatorSim {

class cRenderObjs {

   friend class SimulationState;
   static bool m_bIsInit;
   cRenderObjs();

public:

   static const GLfloat BUILDING_GAP_HEIGHT;
   static const GLfloat ELEV_GAP_WIDTH;
   static const GLfloat GFX_ELEV_SCALE_HEIGHT;
   static const GLfloat GFX_ELEV_SCALE_WIDTH;
   static const GLfloat GFX_FLOOR_QUEUE_SCALE_WIDTH;

   enum OBJ_LIST {
      OBJ_PLANE = 1,
      OBJ_CUBE,
      OBJ_SPHERE,
      OBJ_ELEVATOR,
      OBJ_HUMAN,
      OBJ_MAX
   };

   void init();
   void initCube();
   void initSphere();
   void initPlane();
   void initElevator();
   void initHuman();

   static void renderOccupants(int num,
            int maxOccupants, bool forFloor = true);
   static void drawBitmapText(char *string,float x,float y,float z);
};

} /* namespace elevatorSim */

#endif //__CRENDEROBJS_HPP_
