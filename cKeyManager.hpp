#ifndef __CKEYMANAGER_H__
#define __CKEYMANAGER_H__

#include "ElevatorSim.hpp"

namespace elevatorSim { 

class cKeyManager {
   const static int MAX_KEY;

private:

   cKeyManager();
   bool* m_bOnceKeyDown;
   bool* m_bOnceKeyUp;
   bool* m_bKeyPress;

public:
   static cKeyManager* GetInstance();

   bool onceKeyUp(int key);
   bool onceKeyDown(int key);
   bool keyPress(int key);

   ~cKeyManager(void);
};

} /* namespace elevatorSim */

#endif /* __CKEYMANAGER_H__ */
