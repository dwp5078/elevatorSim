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

#include "ElevatorSim.hpp"
#include "Logger.hpp"

#include <boost/test/included/unit_test.hpp>
#include <boost/test/detail/log_level.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream>
#include <fstream>

static const char UNIT_TEST_LOGFILE_NAME[] = "ElevatorSimUnitTests.log";

struct ElevatorSimUnitTestConfig {
   ElevatorSimUnitTestConfig() :
      test_log(
         UNIT_TEST_LOGFILE_NAME,
         std::ios_base::out | std::ios_base::app ) {
            boost::posix_time::ptime date_time =
               boost::posix_time::microsec_clock::universal_time();

            test_log
               << std::endl
               << "starting test run at "
               << date_time
               << std::endl;

            boost::unit_test::unit_test_log.set_stream( test_log );
            boost::unit_test::unit_test_log.set_threshold_level(
               boost::unit_test::log_successful_tests );
         }

   ~ElevatorSimUnitTestConfig() {
      boost::unit_test::unit_test_log.set_stream( std::cout );
      elevatorSim::Logger::release();
   }

   std::ofstream test_log;
};

boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] ) {
   (void)argc;
   (void)argv;

   elevatorSim::Logger::acquire();
   return NULL;
}

BOOST_GLOBAL_FIXTURE( ElevatorSimUnitTestConfig )
