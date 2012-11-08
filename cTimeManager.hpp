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

#ifndef __CTIMEMANAGER_H__
#define __CTIMEMANAGER_H__

#include "ElevatorSim.hpp"
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
