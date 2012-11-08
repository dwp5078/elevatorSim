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

#ifndef __CCAMERAMANAGER_H__
#define __CCAMERAMANAGER_H__

#include "ElevatorSim.hpp"
#include "cKeyManager.hpp"
#include "cTimeManager.hpp"
#include "MVectors.hpp"

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
   const cTimeManager& timeManager;

public:
   cCameraManager(
         const cKeyManager& _keyManager,
         const cTimeManager& _timeManager
   );

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

