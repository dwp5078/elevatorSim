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

#include "ElevatorSim.hpp"
#include "Logger.hpp"

#include <iostream>
#include <cassert>
#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>

namespace elevatorSim {

   Logger* Logger::loggerInstance = NULL;

   char Logger::logFileName[] = "elevatorSim.log";
   std::ofstream* Logger::logFile = NULL;
   const int Logger::LOG_MSG_LEN_MAX = 1024;

   const char * const Logger::LOG_SUBSYSTEM_NAMES[] = {
      "General",
      "Memory",
      "Elevator Logic",
      "Render",
      "FLTK"
   };

   enum Logger::LOG_LEVEL * Logger::LOG_OUTPUT_LEVELS = NULL;

   Logger::Logger() {
      std::cout << "LOGGER_CREATED" << std::endl;
   }

   Logger::~Logger() {
      std::cout << "LOGGER_DESTROYED" << std::endl;
   }

   Logger& Logger::acquire() {
      if( loggerInstance == NULL ) {
         loggerInstance = new Logger();
         logFile = new std::ofstream(
            logFileName, std::ios_base::out | std::ios_base::app);
         LOG_OUTPUT_LEVELS = new Logger::LOG_LEVEL[_SUB_MAX];

         /* initialize log levels to >= WARNING */
         loggerInstance->setAllSubsystems(LOG_WARNING);
      }

      return *loggerInstance;
   }

   void Logger::release() {
      assert(loggerInstance != NULL && logFile != NULL);

      logFile->close();

      delete [] LOG_OUTPUT_LEVELS;
      delete loggerInstance;
      delete logFile;

      loggerInstance = NULL;
      logFile = NULL;
   }

   void Logger::_logMessage(
      LOG_LEVEL level,
      LOG_SUBSYSTEM system,
      const char* const file,
      const int line,
      const char* const function,
      const std::string& message ) {
         if( level >= LOG_OUTPUT_LEVELS[system] ) {
            char logMsgBuffer[LOG_MSG_LEN_MAX];

            memset(logMsgBuffer, '\0', LOG_MSG_LEN_MAX);

            std::stringstream dbgStream;
            std::ostream& conOstream
               = (level != LOG_ERROR ) ? ( std::cout ) : ( std::cerr );

            dbgStream << "sub " <<  LOG_SUBSYSTEM_NAMES[system] << " in "
               << file << " @ line "
               << line << " in func "
               << function << " : "
               << message << std::endl;

            dbgStream.getline(logMsgBuffer, LOG_MSG_LEN_MAX);

            conOstream << logMsgBuffer << std::endl;
            *logFile << logMsgBuffer << std::endl;
         }
   }

   void Logger::setAllSubsystems(LOG_LEVEL level) {
      assert(level != _LOG_LEVEL_MAX);

      for( int ss = SUB_GENERAL; ss < _SUB_MAX; ++ss ) {
         setSubsystemToLevel((LOG_SUBSYSTEM)ss, level);
      }
   }

   void Logger::setSubsystemToLevel(LOG_SUBSYSTEM system, LOG_LEVEL level) {
      assert( system != _SUB_MAX && level != _LOG_LEVEL_MAX);

      LOG_OUTPUT_LEVELS[system] = level;
   }

} /* namepsace elevatorSim */
