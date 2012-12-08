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
const int ElevatorSimWelcomeWindow::WINDOW_WIDTH = 300;
const int ElevatorSimWelcomeWindow::WINDOW_HEIGHT = 200;
const char ElevatorSimWelcomeWindow::TIPS_CATALOG_FILENAME[] = "Tipscatalog.txt";

void ElevatorSimWelcomeWindow::buildDialogs() {
   confirmDialog = new Fl_Window(350, 160, "");
   Wel_Label = new Fl_Box(10,10,330,50,
      "This Welcome window will not be shown again.\nClose it?");
   OK_Button = new Fl_Button(100, 70, 150, 40, "OK");
   Cancel_Button = new Fl_Button(100, 110, 150, 40, "Cancel");

   OK_Button->callback((Fl_Callback*) quitConfirmedCB, this);
   Cancel_Button->callback((Fl_Callback*) quitCancelledCB, this);

   confirmDialog->add(Wel_Label);
   confirmDialog->add(OK_Button);
   confirmDialog->add(Cancel_Button);
   confirmDialog->end();
}

int ElevatorSimWelcomeWindow::handle(int event) {
   if( event == FL_SHOW ) {
      firstRun = false;
   }

   return Fl_Window::handle(event);
}

void ElevatorSimWelcomeWindow::quitConfirmedCB(
   Fl_Button* okButton, void* userData) {
      (void) okButton;

      ElevatorSimWelcomeWindow* thisWin = (ElevatorSimWelcomeWindow*) userData;

	  std::ofstream fout(FIRST_RUN_FILENAME, std::ios::basic_ios::out);

	  if(fout.good()) {

		  if(thisWin->Check_Button->value())
		  {
			  fout << "T";  
		  } else {
			  fout << "F";
		  }

		  fout.close();
	  } else {
		  assert(false);
	  }

      thisWin->confirmDialog->hide();
      thisWin->hide();
}

void ElevatorSimWelcomeWindow::quitCancelledCB(
   Fl_Button* cancelButton, void* userData) {
   (void) cancelButton;

   ElevatorSimWelcomeWindow* thisWin = (ElevatorSimWelcomeWindow*) userData;
   thisWin->confirmDialog->hide();
}


void ElevatorSimWelcomeWindow::windowCloseCB(Fl_Window* win, void* userData) {
   (void) win;

   ElevatorSimWelcomeWindow* thisWin = (ElevatorSimWelcomeWindow*) userData;
   thisWin->confirmDialog->hotspot(15, 15);
   thisWin->confirmDialog->show();
}

void ElevatorSimWelcomeWindow::writeDatFile() {
   std::ofstream fout(FIRST_RUN_FILENAME, std::ios::basic_ios::out);
   
   if(fout.good()) {
      fout.write("T", 2);
      fout.close();
   } else {
      assert(false);
   }
}

void ElevatorSimWelcomeWindow::readDatFile() {
   std::ifstream fin(FIRST_RUN_FILENAME, std::ios::basic_ios::in );
   
   if(fin.fail()) {
      firstRun = true;
      fin.close();
      writeDatFile();
   } else {
      char firstRunChar;
      fin.read(&firstRunChar, sizeof(firstRunChar));
	  if(firstRunChar == 'T')//if T,means continue show the first run windows
	  {
		firstRun = true;
	  }
	  else//else means does not need to continue show the first run windows
	  {
		firstRun = false;
	  }
      fin.close();
   }
}

void ElevatorSimWelcomeWindow::createTips()
{	
   std::ifstream fin_tips(TIPS_CATALOG_FILENAME, std::ios::basic_ios::in );   

   if(fin_tips.fail()) 
   {

      fin_tips.close();
   } 
   else 
   {
	   char lineTemp[100][200];
   	   int i = 0;
	   int rand_number = 0;

	   while(fin_tips.getline(lineTemp[i],200))//!fin_tips.eof() )
		{
			i++;
		}
      fin_tips.close();

	  rand_number = (rand() % i);
	  WelcomeTextBuffer->append(lineTemp[rand_number]);
   }  
}

/* public methods */
ElevatorSimWelcomeWindow::ElevatorSimWelcomeWindow() :
   Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE) {
      //initial questions&answers
	   WelcomeTextBuffer = new Fl_Text_Buffer();
	    Check_Button = new Fl_Check_Button(10,170,210,20,"Continue showing these tips?");
	   readDatFile();
       createTips();
      /* add more widgets to main window here */
   
      buildDialogs();	  
	  
	  WelcomeDisplay = new Fl_Text_Display(10,30,280,130);
	  WelcomeDisplay->buffer(WelcomeTextBuffer);
	 
      Check_Button->value(1);

	  add(WelcomeDisplay);
      WelcomeDisplay->buffer(WelcomeTextBuffer);
	  add(Check_Button);
      end();

      callback((Fl_Callback*)windowCloseCB, this);
}

ElevatorSimWelcomeWindow::~ElevatorSimWelcomeWindow() {
   delete Wel_Label;
   delete OK_Button;
   delete Cancel_Button;

   delete confirmDialog;
}

} /* namespace elevatorSim */
