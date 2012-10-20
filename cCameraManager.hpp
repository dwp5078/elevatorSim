#ifndef __CCAMERAMANAGER_H__
#define __CCAMERAMANAGER_H__

#include "ElevatorSim.hpp"

class cCameraManager
{
private:
   Vec3f	m_vecCamPos;
   Vec3f	m_vecCamLookAt;
   Vec3f	m_vecCamUp;

public:
   cCameraManager(void);
   ~cCameraManager(void);

   void Update();

   void Yaw(float fAngle);
   void Pitch(float fAngle);
   void Roll(float fAngle);

   inline Vec3f GetCameraPos()   {  return m_vecCamPos;  }
   inline Vec3f GetCameraLookAt()   {  return m_vecCamLookAt;  }
   inline Vec3f GetCameraUp()   {  return m_vecCamUp;  }
};

#endif //__CCAMERAMANAGER_H__