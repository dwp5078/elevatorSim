#include "cTimeManager.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace elevatorSim {

cTimeManager::cTimeManager() {

}

cTimeManager::~cTimeManager() {

}

void cTimeManager::setup() {
   m_dwCurrTime = boost::posix_time::second_clock::local_time();
}

void cTimeManager::update() {
   m_dwPrevTime = m_dwCurrTime;
   m_dwCurrTime = boost::posix_time::second_clock::local_time();
   m_dwElapsedTime = m_dwCurrTime - m_dwPrevTime;
   m_dwWorldTime += m_dwElapsedTime;
}

} /* namespace elevatorSim */
