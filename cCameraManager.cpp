#include "cCameraManager.hpp"
#include "cKeyManager.hpp"

#include "cTimeManager.hpp"

namespace elevatorSim {
const float MOVE = 0.07f;
const float ROT = 0.1f;


cCameraManager::cCameraManager(void)
{
   ReinitCamera();
}


cCameraManager::~cCameraManager(void)
{
}


void cCameraManager::Update()
{
   /*float move = MOVE * cTimeManager::GetInstance()->GetElapsedTime();
   float rot = ROT * cTimeManager::GetInstance()->GetElapsedTime();*/

   //Vec3f right = GetRight() * move;

   /*
   if(cKeyManager::GetInstance()->keyPress(FL_Page_Up))    {
      //m_vecCamPos += m_vecCamUp * move;
      //m_vecCamLookAt += m_vecCamUp * move;
      m_vecCamPos.SetY(m_vecCamPos.y() + move);
      m_vecCamLookAt.SetY(m_vecCamLookAt.y() + move);
   }

   if(cKeyManager::GetInstance()->KeyPress(FL_Page_Down))    {
      //m_vecCamPos -= m_vecCamUp * move;
      //m_vecCamLookAt -= m_vecCamUp * move;
      m_vecCamPos.SetY(m_vecCamPos.y() - move);
      m_vecCamLookAt.SetY(m_vecCamLookAt.y() - move);
   }

   if(cKeyManager::GetInstance()->KeyPress(FL_Right))    {
      //m_vecCamPos -= right * move;
      //m_vecCamLookAt -= right * move;
      m_vecCamPos.SetX(m_vecCamPos.x() + move);
      m_vecCamLookAt.SetX(m_vecCamLookAt.x() + move);
   }

   if(cKeyManager::GetInstance()->KeyPress(FL_Left))    {
      //m_vecCamPos += right * move;
      //m_vecCamLookAt += right * move;
      m_vecCamPos.SetX(m_vecCamPos.x() - move);
      m_vecCamLookAt.SetX(m_vecCamLookAt.x() - move);
   }

   if(cKeyManager::GetInstance()->KeyPress(FL_Up))    {
      m_vecCamPos.SetZ(m_vecCamPos.z() - move);
   }

   if(cKeyManager::GetInstance()->KeyPress(FL_Down))    {
      m_vecCamPos.SetZ(m_vecCamPos.z() + move);
   }

   if(cKeyManager::GetInstance()->KeyPress('W'))      Pitch(-rot);
   if(cKeyManager::GetInstance()->KeyPress('S'))      Pitch(rot);

   if(cKeyManager::GetInstance()->KeyPress('A'))      Yaw(-rot);
   if(cKeyManager::GetInstance()->KeyPress('D'))      Yaw(rot);

   if(cKeyManager::GetInstance()->KeyPress('R'))      Roll(-rot);
   if(cKeyManager::GetInstance()->KeyPress('F'))      Roll(rot);

   if(cKeyManager::GetInstance()->OnceKeyDown(' ')) ReinitCamera();
    */
}


void cCameraManager::Render()
{
   glRotatef(m_fPitchAngle, 1, 0, 0);
   glRotatef(m_fYawAngle, 0, 1, 0);
   glRotatef(m_fRollAngle, 0, 0, 1);

   gluLookAt(m_vecCamPos.x(), m_vecCamPos.y(), m_vecCamPos.z(),
         m_vecCamLookAt.x(), m_vecCamLookAt.y(), m_vecCamLookAt.z(),
         m_vecCamUp.x(), m_vecCamUp.y(), m_vecCamUp.z());
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
