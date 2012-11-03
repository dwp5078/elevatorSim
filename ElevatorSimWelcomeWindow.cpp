/*
 * Copyright (c) 2012, Joseph Max DeLiso
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the FreeBSD Project.
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

namespace elevatorSim {

/* private methods */
/*int ElevatorSimWelcomeWindow::handle(int event) {
   int lastKey = Fl::event_key();

   if(isDebugBuild()) {
      std::cout << "MainWin: event: " << fl_eventnames[event] << std::endl;
   }

   if(event == FL_KEYDOWN)  {
      if(lastKey == FL_Escape) {
         confirmDialog->hotspot(15, 15);
         confirmDialog->show();
      }

      keyManager.down(lastKey);
      return true;
   } else if ( event == FL_KEYUP) {
      keyManager.up(lastKey);
      return true;
   }

   return Fl_Window::handle(event);
}*/


/* private static methods */

		
void ElevatorSimWelcomeWindow::quitConfirmedCB(Fl_Button* OK_Button, void* userData) 
{
   ElevatorSimWelcomeWindow* thisWin = (ElevatorSimWelcomeWindow*) userData;

   thisWin->confirmDialog->hide();
   thisWin->hide();
}

void ElevatorSimWelcomeWindow::quitCancelledCB(Fl_Button* Cancle_Button, void* userData) 
{
   ElevatorSimWelcomeWindow* thisWin = (ElevatorSimWelcomeWindow*) userData;
   thisWin->confirmDialog->hide();
}

/*void ElevatorSimWelcomeWindow::buildButtons(){
   Fl_Button *startButton = new Fl_Button(10, 35, 100, 20, "Begin");
   Fl_Button *pauseButton = new Fl_Button(10, 65, 100, 20, "Pause");
   Fl_Button *stopButton = new Fl_Button(10, 95, 100, 20, "Stop");

   startButton ->when( FL_LEFT_MOUSE);
   pauseButton->when(FL_LEFT_MOUSE);
   stopButton->when(FL_LEFT_MOUSE);

   startButton->clear_visible_focus();
   pauseButton->clear_visible_focus();
   stopButton->clear_visible_focus();

   startButton->callback((Fl_Callback *)startSimCB, this);
   pauseButton->callback((Fl_Callback *)pauseSimCB, this);
   stopButton->callback((Fl_Callback *)stopSimCB, this);
}*/
void ElevatorSimWelcomeWindow::windowCloseCB(Fl_Window* w, void* userData) 
{
   ElevatorSimWelcomeWindow* thisWin = (ElevatorSimWelcomeWindow*) userData;
   thisWin->confirmDialog->hotspot(15, 15);
   thisWin->confirmDialog->show();
}

void ElevatorSimWelcomeWindow::buildDialogs()
{
   /*Welcome Lable*/
	WelcomeLabel = new Fl_Text_Display(10,30,280,190,"Welcome to\nElevatorSimulation Program!");
	add(WelcomeLabel);
	/* Confirmation dialog */
   confirmDialog = new Fl_Window(350, 160, "");
   Wel_Label = new Fl_Box(10,10,330,50,"This Welcome window will not be showed again.\nClose it?");//new Fl_Text_Display(10,10,200,50,"");//"This Welcome window will not be showed again.\r\nContinue?");
   OK_Button = new Fl_Button(100, 70, 150, 40, "OK");
   Cancel_Button = new Fl_Button(100, 110, 150, 40, "Cancel");
      
   OK_Button->callback((Fl_Callback*) quitConfirmedCB, this);
   Cancel_Button->callback((Fl_Callback*) quitCancelledCB, this);

   confirmDialog->add(Wel_Label);
   confirmDialog->add(OK_Button);
   confirmDialog->add(Cancel_Button);
   confirmDialog->end();
}

/* public static member initializers */
const char ElevatorSimWelcomeWindow::WINDOW_TITLE[] = "Welcome";
const int ElevatorSimWelcomeWindow::WINDOW_WIDTH = 300;
const int ElevatorSimWelcomeWindow::WINDOW_HEIGHT = 200;
//const int ElevatorSimWelcomeWindow::MENUBAR_HEIGHT = 25;

/* public methods */
ElevatorSimWelcomeWindow::ElevatorSimWelcomeWindow(int W, int H, const char* Label) :
         Fl_Window(WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_TITLE)
{
   /* add more widgets to main window here */

   //end();
	firstRun = TRUE;

   buildDialogs();

   callback((Fl_Callback*)windowCloseCB, this);

}

ElevatorSimWelcomeWindow::~ElevatorSimWelcomeWindow() 
{
   delete confirmDialog;
   //delete helpWin;
}

} /* namespace elevatorSim */

