#ifndef __CRENDEROBJS_HPP_
#define __CRENDEROBJS_HPP_

#include "ElevatorSim.hpp"

namespace elevatorSim   {

class cRenderObjs
{
   bool m_bIsInit;
   int m_nNumberOfFloor;
   int m_nNumberOfElevator;
   const GLfloat  buildingGapHeight;
   const GLfloat  elevGapWidth;
   
public:

   static enum OBJ_LIST {
      OBJ_PLANE = 1,
      OBJ_CUBE,
      OBJ_BUILDING,
      OBJ_ELEVATOR,
      OBJ_MAX,
   };

   cRenderObjs(int _floors = 5, int _elevators = 20, const GLfloat  _buildingGapHeight = 10.0f, const GLfloat  _buildingGapWidth = 10.0f);
   ~cRenderObjs(void);
   
   void init();
   void initCube();
   void initPlane();
   void initBuilding();
   void initElevator();
};

} /* namespace elevatorSim */

#endif //__CRENDEROBJS_HPP_
