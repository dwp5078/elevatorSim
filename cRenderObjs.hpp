#ifndef __CRENDEROBJS_HPP_
#define __CRENDEROBJS_HPP_

#include "ElevatorSim.hpp"

namespace elevatorSim   {

class cRenderObjs
{
   bool m_bIsInit;
   int m_nNumberOfFloor;
   int m_nNumberOfElevator;

   GLfloat  buildingGapHeight;
   GLfloat  elevGapWidth;
   
public:

   static enum OBJ_LIST {
      OBJ_PLANE = 1,
      OBJ_CUBE,
      OBJ_BUILDING,
      OBJ_ELEVATOR,
      OBJ_MAX,
   };

   cRenderObjs();
   ~cRenderObjs(void);
   
   void init();
   void initCube();
   void initPlane();
   void initBuilding();
   void initElevator();
};

} /* namespace elevatorSim */

#endif //__CRENDEROBJS_HPP_
