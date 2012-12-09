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
#include "ElevatorSimWelcomeWindow.hpp"

#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Enumerations.H>
#include <FL/names.h>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
namespace elevatorSim {

   /* public static member initializers */
   const char ElevatorSimWelcomeWindow::FIRST_RUN_FILENAME[] = ".firstrun";
   const char ElevatorSimWelcomeWindow::WINDOW_TITLE[] = "Welcome";
   const int ElevatorSimWelcomeWindow::WINDOW_WIDTH = 440;
   const int ElevatorSimWelcomeWindow::WINDOW_HEIGHT = 272;
   const char ElevatorSimWelcomeWindow::TIPS_CATALOG_FILENAME[] = "tipsCatalog.txt";

   int ElevatorSimWelcomeWindow::handle(int event) {
      if( event == FL_SHOW ) {
         readDatFile();
         selectTipFromFile();
      } else if ( event == FL_HIDE ) {
         /* check the check box and maybe write the datfile */
         writeDatFile();
         hidden = true;
      }

      return Fl_Window::handle(event);
   }

   void ElevatorSimWelcomeWindow::writeDatFile() {
      std::ofstream fout(FIRST_RUN_FILENAME, 
         std::ios::basic_ios::out | 
         std::ios::basic_ios::trunc);

      if(fout.good()) {
         if( checkButton->value() ) {
            fout << "T";
         } else {
            fout << "F";
         }

         fout.close();
      } 
   }

   void ElevatorSimWelcomeWindow::readDatFile() {
      std::ifstream fin(FIRST_RUN_FILENAME, std::ios::basic_ios::in );

      if(fin.fail()) {
         showTips = true;
         fin.close();
         writeDatFile();
      } else {
         char firstRunChar;
         fin >> firstRunChar;

         /* if T,means continue show the first run window */
         if(firstRunChar == 'T') {
            showTips = true;
            /* else means does not need to continue show the first run window */
         } else {
            showTips = false;
         }

         fin.close();
      }
   }

   void ElevatorSimWelcomeWindow::selectTipFromFile() {
      std::ifstream fin_tips(TIPS_CATALOG_FILENAME, std::ios::basic_ios::in );   

      if(fin_tips.fail()) {
         fin_tips.close();
      } else {
         std::string lineBuffer;
         std::vector<std::string> allTips;

         while(getline(fin_tips, lineBuffer)) {
            allTips.push_back(lineBuffer);
         }

         assert(allTips.size() > 0);
         fin_tips.close();
         welcomeTextBuffer->append(allTips[ (rand() % allTips.size()) ].c_str());
      }  
   }

   /* public methods */
   ElevatorSimWelcomeWindow::ElevatorSimWelcomeWindow() :
   Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE) {

      checkButton = new Fl_Check_Button(
         10, 240, 210, 20, "Continue showing these tips?");
      welcomeTextBuffer = new Fl_Text_Buffer();
      welcomeDisplay = new Fl_Text_Display(10,30,420,200);
      welcomeDisplay->buffer(welcomeTextBuffer);

      end();

      checkButton->value(1);
      readDatFile();
      hidden = false;
   }

   ElevatorSimWelcomeWindow::~ElevatorSimWelcomeWindow() {
      delete checkButton;
      delete welcomeDisplay;
      delete welcomeTextBuffer;
   }

} /* namespace elevatorSim */
