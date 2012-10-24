#ifndef __CTIMEMANAGER_H__
#define __CTIMEMANAGER_H__

#include <boost/date_time/posix_time/posix_time.hpp>

namespace elevatorSim {

class cTimeManager {

   boost::posix_time::ptime m_dwPrevTime;
   boost::posix_time::ptime m_dwCurrTime;

   boost::posix_time::time_duration m_dwWorldTime;
   boost::posix_time::time_duration m_dwElapsedTime;
   boost::posix_time::ptime m_dwLimitTime;

public:

   cTimeManager();
   ~cTimeManager();

   inline boost::posix_time::time_duration elapsedTime() { return m_dwElapsedTime; }
   inline boost::posix_time::time_duration worldTime() { return m_dwWorldTime; }

   void setup();
   void update();

};
} /* namespace elevatorSim */

#endif /* __CTIMEMANAGER_H__ */
