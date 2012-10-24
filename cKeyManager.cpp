#include <FL/Fl.h>

#include "ElevatorSim.hpp"
#include "cKeyManager.hpp"

namespace elevatorSim {

   const int cKeyManager::MAX_KEY = 256;

   cKeyManager::cKeyManager() {
      m_bOnceKeyDown = new bool[MAX_KEY];
      m_bOnceKeyUp = new bool[MAX_KEY];
      m_bKeyPress = new bool[MAX_KEY];

      assert( 
         m_bOnceKeyDown && 
         m_bOnceKeyUp && 
         m_bKeyPress);

      memset(m_bOnceKeyDown, 0, MAX_KEY);
      memset(m_bOnceKeyUp, 0, MAX_KEY);
      memset(m_bKeyPress, 0, MAX_KEY);
   }

   cKeyManager::~cKeyManager() {
      delete [] m_bKeyPress;
      delete [] m_bOnceKeyUp;
      delete [] m_bOnceKeyDown;
   }

   cKeyManager* cKeyManager::GetInstance() {
      static cKeyManager Instance;
      return &Instance;
   }

   bool cKeyManager::onceKeyUp(int key) {
      if(Fl::event_key(key)) {
         m_bOnceKeyUp[key] ^= true;
         return true;
      } else {
         return false;
      }
   }

   bool cKeyManager::onceKeyDown(int key) {
      if(Fl::event_key(key)) {
         m_bOnceKeyDown[key] ^= 1;
         return true; 
      } else {
         return false;
      }
   }

   bool cKeyManager::keyPress(int key) {
      /* TODO: figure out why this is even here */
      if(Fl::event_key(key)) {
         return true;
      } else {
         return false;
      }
   }

} /* namespace elevatorSim */
