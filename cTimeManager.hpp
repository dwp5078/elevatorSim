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

   size_t totalFrames;
   boost::posix_time::time_duration frameLimit;

public:

   static const boost::posix_time::time_duration redrawInterval;

   cTimeManager();

   void reset();
   void update();

   /* public const inline getters */
   inline boost::posix_time::time_duration worldTime() const { 
      return m_dwWorldTime; 
   }

   inline size_t getFPS() const {
      return 60; /* TODO */
   }

   inline size_t getTotalFrames() const {
      return 1; /* TODO */
   }

   inline boost::posix_time::time_duration getLastFrameTime() const {
      return m_dwLastFrameTime;
   }
};

} /* namespace elevatorSim */

#endif /* __CTIMEMANAGER_H__ */
