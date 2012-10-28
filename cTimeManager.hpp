#ifndef __CTIMEMANAGER_H__
#define __CTIMEMANAGER_H__

#include <boost/date_time/posix_time/posix_time.hpp>

namespace elevatorSim {

class cTimeManager {

   boost::posix_time::ptime m_dwPrevTime;
   boost::posix_time::ptime m_dwCurrTime;

   boost::posix_time::time_duration m_dwWorldTime;
   boost::posix_time::time_duration m_dwLastFrameTime;
   boost::posix_time::ptime m_dwLimitTime;

   size_t FPS;
   size_t totalFrames;
   size_t secondFrames;
   boost::posix_time::time_duration frameLimit;

public:

   static const double redrawInterval;

   cTimeManager();

   void reset();
   void update();

   /* public const inline getters */
   inline boost::posix_time::time_duration worldTime() const { 
      return m_dwWorldTime; 
   }

   inline size_t getFPS() const {
      return FPS;  
   }

   inline size_t getTotalFrames() const {
      return totalFrames;
   }

   inline boost::posix_time::time_duration getLastFrameTime() const {
      return m_dwLastFrameTime;
   }
};

} /* namespace elevatorSim */

#endif /* __CTIMEMANAGER_H__ */

