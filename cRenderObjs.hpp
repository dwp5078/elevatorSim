#ifndef __CRENDEROBJS_HPP_
#define __CRENDEROBJS_HPP_

#include "ElevatorSim.hpp"

#include <GL/glut.h>

namespace elevatorSim   {

class cRenderObjs
{
   bool m_bIsInit;

public:

   static const GLfloat BUILDING_GAP_HEIGHT;
   static const GLfloat ELEV_GAP_WIDTH;
   static const GLfloat GFX_ELEV_SCALE_HEIGHT;
   static const GLfloat GFX_ELEV_SCALE_WIDTH;

   enum OBJ_LIST {
      OBJ_PLANE = 1,
      OBJ_CUBE,
      OBJ_SPHERE,
      OBJ_ELEVATOR,
      OBJ_HUMAN,
      OBJ_MAX
   };

   cRenderObjs();

   void init();
   void initCube();
   void initSphere();
   void initPlane();
   void initElevator();
   void initHuman();
};

} /* namespace elevatorSim */

#endif //__CRENDEROBJS_HPP_

