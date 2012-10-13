#include "cTimeManager.hpp"
#include <ctime>

cTimeManager::cTimeManager() {

}

cTimeManager::~cTimeManager() {

}

cTimeManager* cTimeManager::GetInstance() {
   static cTimeManager Instance;

   return &Instance;
}

void cTimeManager::Setup()
{
   m_dwCurrTime = clock();
   m_dwPrevTime = m_dwCurrTime;

   m_dwWorldTime = 0;
   m_dwElapsedTime = 0;
   m_dwFps = 0;
   m_dwFrame = 0;

   m_dwLimitTime = 0;
   m_dwFpsPlus = 0;
}

void cTimeManager::Update()
{
   m_dwPrevTime = m_dwCurrTime;
   m_dwCurrTime = clock();

   m_dwElapsedTime = m_dwCurrTime - m_dwPrevTime;
   m_dwWorldTime += m_dwElapsedTime;

   if(m_dwLimitTime < m_dwWorldTime)
   {
      m_dwLimitTime = m_dwWorldTime + CLOCKS_PER_SEC;
      m_dwFps = m_dwFrame;
      m_dwFpsPlus += m_dwFrame;
      m_dwFrame = 0;
   }

   else
   {
      m_dwFrame++;
   }
}
