#ifndef __CCAMERAMANAGER_H__
#define __CCAMERAMANAGER_H__

#include "ElevatorSim.hpp"
#include "cKeyManager.hpp"

namespace elevatorSim {

   class cCameraManager {
   private:
      Vec3f	m_vecCamPos;
      Vec3f	m_vecCamLookAt;
      Vec3f	m_vecCamUp;

      float m_fPitchAngle;
      float m_fYawAngle;
      float m_fRollAngle;

      const cKeyManager& keyManager;

   public:
      cCameraManager(const cKeyManager& _keyManager);

      void Update();
      void Render();

      void Yaw(float fAngle);
      void Pitch(float fAngle);
      void Roll(float fAngle);

      void ReinitCamera();

      Vec3f GetRight();

      inline Vec3f GetCameraPos() {
         return m_vecCamPos;
      }

      inline Vec3f GetCameraLookAt() {
         return m_vecCamLookAt;
      }

      inline Vec3f GetCameraUp() {
         return m_vecCamUp;
      }
   }; /* cCameraManager */

} /* namespace elevatorSim */

#endif //__CCAMERAMANAGER_H__
