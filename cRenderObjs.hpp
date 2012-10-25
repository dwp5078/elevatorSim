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
      int   m_nNumberOfFloor;
      int   m_nNumberOfElevator;

      void Init();
      void InitCube();
      void InitPlane();
      void InitBuilding();
      void InitElevator();
   };
}

#endif //__CRENDEROBJS_HPP_