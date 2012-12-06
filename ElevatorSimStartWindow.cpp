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
#include "ElevatorSimStartWindow.hpp"
#include "SimulationState.hpp"
#include "Logger.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>

#include <boost/lexical_cast.hpp>
#include <fstream>

namespace elevatorSim {

/* public static member initializers */
const char ElevatorSimStartWindow::WINDOW_TITLE[] = "Start";
const int ElevatorSimStartWindow::WINDOW_WIDTH = 310;
const int ElevatorSimStartWindow::WINDOW_HEIGHT = 320;

int ElevatorSimStartWindow::handle(int event) {
   return Fl_Window::handle(event);
}

void ElevatorSimStartWindow::browseCB(Fl_Button* b, void* userData) {
   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "button @ " << b << std::endl;
      LOG_INFO( Logger::SUB_FLTK, sstreamToBuffer(dbgSS) );
   }

   ElevatorSimStartWindow* thisWindow = (ElevatorSimStartWindow*) userData;
   thisWindow -> elevatorAIFileChooser -> show();
}

void ElevatorSimStartWindow::inputAcceptCB(Fl_Window* w, void* userData) {
   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "inputAcceptCB fired with widget ptr " << w << std::endl;
      LOG_INFO( Logger::SUB_FLTK, sstreamToBuffer(dbgSS) );
   }

   (void) w;
   ElevatorSimStartWindow* thisWindow = (ElevatorSimStartWindow*) userData;

   try {
      int elevatorCount = 
         boost::lexical_cast<int> ( thisWindow->elevatorNumInput->value() );
      int floorCount = 
         boost::lexical_cast<int> ( thisWindow->floorNumInput->value() );
      int randomSeed = 
         boost::lexical_cast<int> ( thisWindow->seedNumInput->value() );
      
      if(isDebugBuild()) {
         std::stringstream dbgSS;
         dbgSS << "in inputAcceptCB with well formed params ec = " 
            << elevatorCount
            << " fc = " << floorCount 
            << " rs = " << randomSeed
            << std::endl;
         LOG_INFO( Logger::SUB_FLTK, sstreamToBuffer(dbgSS) );
      }

      if( elevatorCount >= 1 && floorCount > 1 ) {
         SimulationState& simState = SimulationState::acquire();
         simState.start(elevatorCount, floorCount, randomSeed, "");
         thisWindow->hide();
      } else {
         LOG_ERROR( Logger::SUB_FLTK, "parameters out of range" );
      }

      /*elevatorAIPathInput->value();*/

   } catch ( boost::bad_lexical_cast& ) {
      LOG_ERROR( Logger::SUB_ELEVATOR_LOGIC, "failed to parse input parameters"); 
   }


}

void ElevatorSimStartWindow::inputCancelCB(Fl_Window* w, void* userData) {
   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "inputCancelCB fired with widget ptr " << w << std::endl;
      LOG_INFO( Logger::SUB_FLTK, sstreamToBuffer(dbgSS) );
   }
   
   (void) w;

   ElevatorSimStartWindow* thisWindow = (ElevatorSimStartWindow*) userData;
   thisWindow->hide();
}

void ElevatorSimStartWindow::fileChosenCB(Fl_File_Chooser* cw, void* userData) {
   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "fileChosen fired with widget ptr " << cw
         << " and value: " << cw->value() << std::endl;
      LOG_INFO( Logger::SUB_FLTK, sstreamToBuffer(dbgSS) );
   }

   ElevatorSimStartWindow* thisWindow = (ElevatorSimStartWindow*) userData;
   (void) thisWindow;

   std::ifstream scriptHandle( cw -> value(), std::ifstream::in );

   if( scriptHandle.fail() ) {
      /* TODO: display an error: couldn't open script file */
   
      if(isDebugBuild()) {
         std::stringstream dbgSS;
         dbgSS << "could not open file loc'd at " << cw -> value() << std::endl;
         LOG_INFO( Logger::SUB_FLTK, sstreamToBuffer(dbgSS) );
      }
   } else {
      scriptHandle.close();
      thisWindow->elevatorAIPathInput->value( cw -> value() );
      cw -> hide();
   }
}

/* public methods */
ElevatorSimStartWindow::ElevatorSimStartWindow() :
   Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE) {
      elevatorNumInput = new Fl_Input(100, 20, 140, 30, "# of elevators:");
      floorNumInput = new Fl_Input(100, 70, 140, 30, "# of floors:");
      seedNumInput = new Fl_Input(100, 120, 140, 30, "Random seed:");
      elevatorAIPathInput = new Fl_Input(100, 170, 140, 30, "AI script: ");
      browseButton = new Fl_Button(100, 220, 140, 30, "Browse...");

      inputCancel = new Fl_Button(10, 270, 140, 30, "Cancel");
      inputAccept = new Fl_Button(160, 270, 140, 30, "Accept");

      browseButton->callback((Fl_Callback*) browseCB, this);
      inputAccept->callback((Fl_Callback*) inputAcceptCB, this);
      inputCancel->callback((Fl_Callback*) inputCancelCB, this);

      add(floorNumInput);
      add(elevatorNumInput);
      add(seedNumInput);
      add(elevatorAIPathInput);
      add(browseButton);
      add(inputCancel);
      add(inputAccept);
      end();

      elevatorAIFileChooser = new Fl_File_Chooser(".", "*.py", Fl_File_Chooser::SINGLE, "specify AI script");
      elevatorAIFileChooser->callback( fileChosenCB, this );
}

ElevatorSimStartWindow::~ElevatorSimStartWindow() {
   delete elevatorAIFileChooser;

   delete inputCancel;
   delete inputAccept;
   delete browseButton;

   delete elevatorAIPathInput;
   delete seedNumInput;
   delete floorNumInput;
   delete elevatorNumInput;

   if(isDebugBuild()) {
      std::stringstream dbgSS;
      dbgSS << "finished destroying start window @ " << this << std::endl;
      LOG_INFO( Logger::SUB_MEMORY, sstreamToBuffer(dbgSS) );
   }
}

} /* namespace elevatorSim */
