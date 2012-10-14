#ifndef __CTIMEMANAGER_H__
#define __CTIMEMANAGER_H__

#include <cstdint>

namespace elevatorSim {

   class cTimeManager {

      uint32_t m_dwPrevTime;
      uint32_t m_dwCurrTime;

      uint32_t m_dwWorldTime;
      uint32_t m_dwElapsedTime;
      uint32_t m_dwFps;
      uint32_t m_dwFrame;

      uint32_t m_dwLimitTime;
      uint32_t m_dwFpsPlus;

      cTimeManager(void);

   public:

      ~cTimeManager(void);

      static cTimeManager* GetInstance();

      inline uint32_t GetElapsedTime() { return m_dwElapsedTime; }
      inline uint32_t GetWorldTime() { return m_dwWorldTime; }
      inline uint32_t GetFPS() { return m_dwFps; }
      inline uint32_t GetTotalFrame() { return m_dwFpsPlus; }

      void Setup();
      void Update();

   };
} /* namespace elevatorSim */

#endif /* __CTIMEMANAGER_H__ */
