#include "cTimeManager.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace elevatorSim {


const double cTimeManager::redrawInterval = 0.001;

cTimeManager::cTimeManager() {
   reset();
}

void cTimeManager::reset() {
   m_dwCurrTime = boost::posix_time::second_clock::local_time();
   m_dwLimitTime = m_dwCurrTime + boost::posix_time::seconds(1);
   totalFrames = 0;
   secondFrames = 0;
}

void cTimeManager::update() {
   m_dwPrevTime = m_dwCurrTime;
   m_dwCurrTime = boost::posix_time::second_clock::local_time();
   m_dwLastFrameTime = m_dwCurrTime - m_dwPrevTime;
   m_dwWorldTime += m_dwLastFrameTime;

   ++totalFrames;
   ++secondFrames;

   if( m_dwCurrTime >= m_dwLimitTime ) {
      FPS = secondFrames;
      secondFrames = 0;
      m_dwLimitTime = m_dwCurrTime + boost::posix_time::seconds(1);
   }
}

} /* namespace elevatorSim */

