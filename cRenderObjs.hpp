#ifndef __CRENDEROBJS_HPP_
#define __CRENDEROBJS_HPP_

#include "ElevatorSim.hpp"

namespace elevatorSim   {
class cRenderObjs
{
public:
   cRenderObjs(void);
   ~cRenderObjs(void);

   bool  m_bIsInit;

   void init();
   void initCube();
   void initPlane();
   void initBuilding();
   void initElevator();
};
}

#endif //__CRENDEROBJS_HPP_
