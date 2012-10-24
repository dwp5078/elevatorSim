#ifndef __CKEYMANAGER_H__
#define __CKEYMANAGER_H__

#include <map>

#include "ElevatorSim.hpp"

namespace elevatorSim { 

class cKeyManager {
private:

   std::map<int, bool> keysPressed;

public:

   inline void up(int key) {
      keysPressed[key] = false;
   }

   inline void down(int key) {
      keysPressed[key] = true;
   }

   inline bool isDown(int key) const {
      std::map<int, bool>::const_iterator cit = keysPressed.find(key);
      
      if(cit == keysPressed.end()) {
         return false;
      } else {
         return cit->second;;
      }
   }
};

} /* namespace elevatorSim */

#endif /* __CKEYMANAGER_H__ */
