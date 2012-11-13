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
#include "ElevatorSimWindow.hpp"
#include "SimulationState.hpp"
#include "Logger.hpp"

#include <boost/version.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include <boost/program_options.hpp>
#include <iostream>
#include <iterator>
#include <string>

using namespace elevatorSim;

void compute();
void parseArgs(int argc, char** argv);

int main(int argc, char** argv) {
   glutInit(&argc, argv);

   Logger::acquire();
   Py_Initialize();

   LOG_INFO(Logger::SUB_GENERAL, "logger starting up");

   /* output version info of libraries */
   std::cout << "built with: " << std::endl
      << "boost v" << BOOST_LIB_VERSION << std::endl
      << "fltk v" << FL_MAJOR_VERSION << "_" 
      << FL_MINOR_VERSION << FL_PATCH_VERSION << std::endl
      << "python v" << PY_MAJOR_VERSION << "_" << PY_MINOR_VERSION << std::endl;
   

   parseArgs(argc, argv);
   srand(time(0)); /* TODO: use Boost.Random */

   SimulationState::acquire();

   ElevatorSimWindow* mainWin = new ElevatorSimWindow();
   mainWin -> show();

   boost::thread computeThread(compute);
   Fl::run();
   computeThread.join();

   delete mainWin;

   LOG_INFO(Logger::SUB_GENERAL, "logger shutting down");

   SimulationState::release();
   Logger::release();
   Py_Finalize();

   return 0;
}

void compute() {
   /* TODO: move this into a different file */
   static boost::chrono::milliseconds waitDuration(50); /* TODO: make this a const */

   boost::this_thread::sleep_for(waitDuration);
}

void parseArgs(int argc, char** argv) {
   using namespace boost::program_options;

   try {
      options_description desc("Allowed options");
      desc.add_options()
         ("help", "produce help message")
         ("verbose", "verbose flag");

      variables_map vm;
      store(parse_command_line(argc, argv, desc), vm);
      notify(vm);

      if (vm.count("help")) {
         std::cout << desc << std::endl;
         return;
      }

      if (vm.count("verbose")) {
         Logger::acquire().setAllSubsystems(Logger::LOG_INFO);
      }
   } catch(std::exception& e) {
      std::cerr << "error: " << e.what() << std::endl;
      return;
   } catch(...) {
      std::cerr << "Exception of unknown type" << std::endl;
   }
}
