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

#include "ElevatorSim.hpp"
#include "cCameraManager.hpp"
#include "cKeyManager.hpp"
#include "cTimeManager.hpp"

#include <FL/Enumerations.H>
#include <GL/glut.h>

namespace elevatorSim {

const float MOVE = 0.37f;
const float ROT = 0.1f;

cCameraManager::cCameraManager(
      const cKeyManager& _keyManager,
      const cTimeManager& _timeManager) :
			            keyManager(_keyManager),
			            timeManager(_timeManager) {
   ReinitCamera();
}


void cCameraManager::Update()
{
   /* TODO: use timeManager.getLastFrameTime().total_milliseconds() */

   float move = MOVE;
   float rot = ROT;

   Vec3f right = GetRight() * move;

   if(keyManager.isDown(FL_Page_Up)) {
      m_vecCamPos.y = m_vecCamPos.y + move;
      m_vecCamLookAt.y = m_vecCamLookAt.y + move;
   }

   if(keyManager.isDown(FL_Page_Down)) {
      m_vecCamPos.y = m_vecCamPos.y - move;
      m_vecCamLookAt.y = m_vecCamLookAt.y - move;
   }

   if(keyManager.isDown(FL_Right)) {
      m_vecCamPos.x = m_vecCamPos.x + move;
      m_vecCamLookAt.x = m_vecCamLookAt.x + move;
   }

   if(keyManager.isDown(FL_Left)) {
      m_vecCamPos.x = m_vecCamPos.x - move;
      m_vecCamLookAt.x = m_vecCamLookAt.x - move;
   }

   if(keyManager.isDown(FL_Up)) {
      m_vecCamPos.z = m_vecCamPos.z - move;
   }

   if(keyManager.isDown(FL_Down)) {
      m_vecCamPos.z = m_vecCamPos.z + move;
   }

   if(keyManager.isDown('w')) {
      Pitch(-rot);
   }

   if(keyManager.isDown('s')) {
      Pitch(rot);
   }

   if(keyManager.isDown('a')) {
      Yaw(-rot);
   }

   if(keyManager.isDown('d')) {
      Yaw(rot);
   }

   if(keyManager.isDown('r')) {
      Roll(-rot);
   }

   if(keyManager.isDown('f')) {
      Roll(rot);
   }

   if(keyManager.isDown(' ')) {
      ReinitCamera();
   }
}


void cCameraManager::Render()
{
   glRotatef(m_fPitchAngle, 1, 0, 0);
   glRotatef(m_fYawAngle, 0, 1, 0);
   glRotatef(m_fRollAngle, 0, 0, 1);

   gluLookAt(m_vecCamPos.x, m_vecCamPos.y, m_vecCamPos.z,
         m_vecCamLookAt.x, m_vecCamLookAt.y, m_vecCamLookAt.z,
         m_vecCamUp.x, m_vecCamUp.y, m_vecCamUp.z);
}

void cCameraManager::Yaw(float fAngle)
{
   m_fYawAngle += fAngle;
}

void cCameraManager::Pitch(float fAngle)
{
   m_fPitchAngle += fAngle;
}

void cCameraManager::Roll(float fAngle)
{
   m_fRollAngle += fAngle;
}

void cCameraManager::ReinitCamera()
{
   m_vecCamPos.Set(0.f, 0.f, 20.f);
   m_vecCamLookAt.Set(0.f, 0.f, 0.f);
   m_vecCamUp.Set(0.0f, 1.0f, 0.0f);

   Vec3f lookingVec = m_vecCamPos - m_vecCamLookAt;
   Vec3f right = GetRight();
   Vec3f::Cross3(m_vecCamUp, right, lookingVec);

   m_fPitchAngle = 0.f;
   m_fYawAngle = 0.f;
   m_fRollAngle = 0.f;
}

Vec3f cCameraManager::GetRight()
{
   Vec3f right;
   Vec3f lookingVec = m_vecCamPos - m_vecCamLookAt;
   lookingVec.Normalize();
   Vec3f::Cross3(right, lookingVec, m_vecCamUp);
   right.Normalize();

   return right;
}

} /* namespace elevatorSim */
