#include "cCameraManager.hpp"

const float MOVE = 0.5f;
const float ROT = 0.3f;

cCameraManager::cCameraManager(void)
{
   m_vecCamPos.Set(0.f, 0.f, 20.f);
   m_vecCamLookAt.Set(0.f, 0.f, 0.f);
   m_vecCamUp.Set(0.0f, 1.0f, 0.0f);

   Vec3f right;
   Vec3f lookingVec = m_vecCamPos - m_vecCamLookAt;
   lookingVec.Normalize();
   Vec3f::Cross3(right, lookingVec, m_vecCamUp);
   Vec3f::Cross3(m_vecCamUp, right, lookingVec);
}

cCameraManager::~cCameraManager(void)
{
}

void cCameraManager::Update()
{
   if(cKeyManager::GetInstance()->KeyPress(FL_Up))    {
      m_vecCamPos.SetY(m_vecCamPos.y() + MOVE);
      m_vecCamLookAt.SetY(m_vecCamLookAt.y() + MOVE);
   }

   if(cKeyManager::GetInstance()->KeyPress(FL_Down))    {
      m_vecCamPos.SetY(m_vecCamPos.y() - MOVE);
      m_vecCamLookAt.SetY(m_vecCamLookAt.y() - MOVE);
   }

   if(cKeyManager::GetInstance()->KeyPress(FL_Right))    {
      m_vecCamPos.SetX(m_vecCamPos.x() + MOVE);
      m_vecCamLookAt.SetX(m_vecCamLookAt.x() + MOVE);
   }

   if(cKeyManager::GetInstance()->KeyPress(FL_Left))    {
      m_vecCamPos.SetX(m_vecCamPos.x() - MOVE);
      m_vecCamLookAt.SetX(m_vecCamLookAt.x() - MOVE);
   }

   if(cKeyManager::GetInstance()->KeyPress(FL_Page_Up))    {
      m_vecCamPos.SetZ(m_vecCamPos.z() - MOVE);
   }

   if(cKeyManager::GetInstance()->KeyPress(FL_Page_Down))    {
      m_vecCamPos.SetZ(m_vecCamPos.z() + MOVE);
   }

   if(cKeyManager::GetInstance()->KeyPress('O'))      Pitch(-ROT);
   if(cKeyManager::GetInstance()->KeyPress('P'))      Pitch(ROT);
}

void cCameraManager::Yaw(float fAngle)
{

}

void cCameraManager::Pitch(float fAngle)
{
   Vec3f right;
   Vec3f::Cross3(right, m_vecCamPos, m_vecCamLookAt);

   
}

void cCameraManager::Roll(float fAngle)
{

}