#include "cKeyManager.hpp"

cKeyManager::cKeyManager(void)
{
   for(int i=0; i<128; i++)
   {
      m_bOnceKeyDown[i]	= false;
      m_bOnceKeyUp[i]		= false;
      m_bKeyPress[i]		= false;
   }
}

cKeyManager::~cKeyManager(void)
{
}

cKeyManager* cKeyManager::GetInstance()
{
   static cKeyManager Instance;
   return &Instance;
}

bool cKeyManager::OnceKeyUp(int key)
{
   if(Fl::focus() == 0) return false;

   if(Fl::event_key(key))
   {
      if(m_bOnceKeyUp[key] == false)
      {
         m_bOnceKeyUp[key] = true;
      }
   }

   else if(m_bOnceKeyUp[key] == true)
   {
      m_bOnceKeyUp[key] = false;
      return true;
   }

   return false;
}

bool cKeyManager::OnceKeyDown(int key)
{
   if(Fl::focus() == 0) return false;
   if(Fl::event_key(key))
   {
      if(m_bOnceKeyDown[key] == false)
      {
         m_bOnceKeyDown[key] = true;
         return true;
      }
   }

   else if(m_bOnceKeyDown[key] == true)
   {
      m_bOnceKeyDown[key] = false;
   }

   return false;
}

bool cKeyManager::KeyPress(int key)
{
   if(Fl::focus() == 0) return false;
   if(Fl::event_key(key))
   {
      return true;
   }

   return false;
}
