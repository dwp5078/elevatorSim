/*
 * Copyright (c) 2012, Joseph Max DeLiso
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

#ifndef _LOGGER_H
#define _LOGGER_H

#include "ElevatorSim.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <map>

namespace elevatorSim {

   class Logger {

      Logger();
      ~Logger();

      static Logger* loggerInstance;
      static std::ofstream* logFile;

   public:

      static char logFileName[];

      enum LOG_LEVEL {
         LOG_INFO,
         LOG_WARNING,
         LOG_ERROR,
         _LOG_LEVEL_MAX
      };

      enum LOG_SUBSYSTEM {
         SUB_GENERAL,
         SUB_MEMORY,
         SUB_ELEVATOR_LOGIC,
         SUB_RENDER,
         SUB_FLTK,
         _SUB_MAX
      };

      static const char * const LOG_SUBSYSTEM_NAMES[];
      static enum LOG_LEVEL * LOG_OUTPUT_LEVELS;
      static const int LOG_MSG_LEN_MAX;

      static Logger& acquire();
      static void release();

      void _logMessage(
         LOG_LEVEL level,
         LOG_SUBSYSTEM system,
         const char* const file,
         const int line,
         const char* const function,
         const std::string& message );

      void setAllSubsystems(LOG_LEVEL level);
      void setSubsystemToLevel(LOG_SUBSYSTEM system, LOG_LEVEL level);
   };

} /* namespace elevatorSim */

#define LOG_INFO(SYSTEM, MSG) Logger::acquire()._logMessage\
   ( Logger::LOG_INFO, SYSTEM, __FILE__, __LINE__, __FUNCTION__, MSG )

#define LOG_WARNING(SYSTEM, MSG) Logger::acquire()._logMessage\
   ( Logger::LOG_WARNING, SYSTEM, __FILE__, __LINE__, __FUNCTION__, MSG )

#define LOG_ERROR(SYSTEM, MSG) Logger::acquire()._logMessage\
   ( Logger::LOG_ERROR, SYSTEM, __FILE__, __LINE__, __FUNCTION__, MSG )

#endif /* _LOGGER_H */
