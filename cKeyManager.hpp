#ifndef __CKEYMANAGER_H__
#define __CKEYMANAGER_H__

#include "ElevatorSim.hpp"


#define	KEYMAX	150

class cKeyManager
{
private:
   cKeyManager(void);
   bool m_bOnceKeyDown[KEYMAX];
   bool m_bOnceKeyUp[KEYMAX];
   bool m_bKeyPress[KEYMAX];

public:
   static cKeyManager* GetInstance();

   bool OnceKeyUp(int key);
   bool OnceKeyDown(int key);
   bool KeyPress(int key);

   ~cKeyManager(void);
};

#endif //__CKEYMANAGER_H__