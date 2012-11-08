/*
 * Copyright (c) 2012, Joseph Max DeLiso, Daniel Gilbert
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are 
 * those of the authors and should not be interpreted as representing official 
 * policies, either expressed or implied, of the FreeBSD Project.
 */

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
