#ifndef __CRENDEROBJS_HPP_
#define __CRENDEROBJS_HPP_

#include "ElevatorSim.hpp"

#include <GL/glut.h>

namespace elevatorSim   {

class cRenderObjs
{
   bool m_bIsInit;
   int m_nNumberOfFloor;
   int m_nNumberOfElevator;

public:

   static const GLfloat BUILDING_GAP_HEIGHT;
   static const GLfloat ELEV_GAP_WIDTH;
   
   enum OBJ_LIST {
      OBJ_PLANE = 1,
      OBJ_CUBE,
      OBJ_BUILDING,
      OBJ_ELEVATOR,
      OBJ_MAX
   };

   cRenderObjs();

   void init();
   void initCube();
   void initPlane();
   void initBuilding();
   void initElevator();
};

} /* namespace elevatorSim */

#endif //__CRENDEROBJS_HPP_

