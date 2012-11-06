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
#include "ElevatorSimWindow.hpp"
#include "ElevatorSimRenderWindow.hpp"

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
int ElevatorSimWindow::handle(int event) {
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
}


/* private static methods */
void ElevatorSimWindow::windowCloseCB(Fl_Window* w, void* userData) {

   if(isDebugBuild()) {
      std::cout << "windowCloseCB fired with widget ptr " << w << std::endl;
   }

   ElevatorSimWindow* thisWin = (ElevatorSimWindow*) userData;
   thisWin->confirmDialog->hotspot(15, 15);
   thisWin->confirmDialog->show();
}

void ElevatorSimWindow::menuNewCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      std::cout << "menuNewCB fired with widget ptr " << w 
         << "and userData " << userData << std::endl;
   }

   /* TODO */
}

void ElevatorSimWindow::menuOpenCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      std::cout << "menuNewCB fired with widget ptr " << w 
         << "and userData " << userData << std::endl;
   }
}

void ElevatorSimWindow::menuHelpCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      std::cout << "menuHelpCB fired with widget ptr " << w << std::endl;
   }

   ElevatorSimWindow* thisWindow = (ElevatorSimWindow*) userData;
   thisWindow->helpWin->hotspot(15, 15);
   thisWindow->helpWin->show();
}

void ElevatorSimWindow::dismissHelpCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      std::cout << "dismissHelpCB fired with widget " << w << std::endl;
   }

   ElevatorSimWindow* thisWindow = (ElevatorSimWindow*) userData;
   thisWindow->helpWin->hide();
}

void ElevatorSimWindow::startSimCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      std::cout << "in startSimCB with widget ptr " << w 
         << " and userData " << userData << std::endl;
   }

   Fl_Button* startButton = (Fl_Button*)w;

   if(startButton->value()) {
      if(isDebugBuild() ) {
         std::cout << "startSim CB fired" << std::endl;
         toggleButtons((ElevatorSimWindow*) userData);
      }

      /* TODO */

   }
}

void ElevatorSimWindow::pauseSimCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      std::cout << "in pauseSimCB with widget ptr " << w 
         << " and userData " << userData << std::endl;
   }

   Fl_Button* pauseButton = (Fl_Button*)w;
   
   /* TODO: this needs to be moved into time manager */
   static bool paused = true;

   if(pauseButton->value()) {
      std::cout << "pauseSim CB fired" << std::endl;

      if(paused) {
         w->label("Resume");
       
      } else {
         w->label("Pause");

      }

      paused = !paused;
   }
}

void ElevatorSimWindow::stopSimCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      std::cout << "in stopSimCB with widget ptr " << w 
         << " and userData " << userData << std::endl;
   }

   Fl_Button* stopButton = (Fl_Button*)w;

   if(stopButton->value()) {
      std::cout << "stopSim CB fired" << std::endl;
         toggleButtons((ElevatorSimWindow*) userData);
   }
}

void ElevatorSimWindow::menuSaveCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      std::cout << "menuSaveCB fired with widget ptr " << w 
         << " and userData " << userData << std::endl;
   }

   /* TODO */
}

void ElevatorSimWindow::menuQuitCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      std::cout << "menuQuitCB fired with widget ptr " << w 
         << " and userData " << userData << std::endl;
   }

   ElevatorSimWindow* thisWin = (ElevatorSimWindow*) userData;
   thisWin->confirmDialog->hotspot(15, 15);
   thisWin->confirmDialog->show();
}

void ElevatorSimWindow::menuAboutCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      std::cout << "menuAboutCB fired with widget ptr " << w 
         << " and userData " << userData << std::endl;
   }
}

void ElevatorSimWindow::quitConfirmedCB(Fl_Button* yesButton, void* userData) {
   if(isDebugBuild()) {
      std::cout << "quitConfirmedCB fired with widget ptr " << yesButton 
         << " and userData " << userData << std::endl;
   }

   ElevatorSimWindow* thisWin = (ElevatorSimWindow*) userData;

   thisWin->confirmDialog->hide();
   thisWin->hide();
}

void ElevatorSimWindow::quitCancelledCB(Fl_Button* noButton, void* userData) {
   if(isDebugBuild()) {
      std::cout << "quitCancelledCB fired with widget ptr " << noButton 
         << " and userData " << userData << std::endl;
   }

   ElevatorSimWindow* thisWin = (ElevatorSimWindow*) userData;
   thisWin->confirmDialog->hide();
}

void ElevatorSimWindow::buildMenu() {
   /*
    * struct Fl_Menu_Item {
    *    const char*          text;     // label()
    *    ulong                shortcut_;
    *    Fl_Callback*         callback_;
    *    void*                user_data_;
    *    int                  flags;
    *    uchar                labeltype_;
    *    uchar                labelfont_;
    *    uchar                labelsize_;
    *    uchar                labelcolor_;
    * };
    */

   static const Fl_Menu_Item menuitems[] = {
         { "&File", 0, 0, 0, FL_SUBMENU },
         { "&Open", FL_COMMAND + 'o', (Fl_Callback *)menuOpenCB, this },
         { "E&xit", FL_COMMAND + 'x', (Fl_Callback *)menuQuitCB, this },
         { 0 },
         { "&Help", 0, 0, 0, FL_SUBMENU },
         { "How to use", 0, (Fl_Callback *)menuHelpCB, this },
         { "About", 0, (Fl_Callback *)menuAboutCB, this },
         { 0 },
         { 0 }};

   Fl_Menu_Bar* menubar = new Fl_Menu_Bar(0, 0, w(), MENUBAR_HEIGHT);
   menubar->copy(menuitems);

   add(menubar);
}

void ElevatorSimWindow::buildButtons(){
   startButton = new Fl_Button(10, 35, 100, 20, "Begin");
   pauseButton = new Fl_Button(10, 65, 100, 20, "Pause");
   stopButton = new Fl_Button(10, 95, 100, 20, "Stop");

   startButton ->when( FL_LEFT_MOUSE);
   pauseButton->when(FL_LEFT_MOUSE);
   stopButton->when(FL_LEFT_MOUSE);

   startButton->clear_visible_focus();
   pauseButton->clear_visible_focus();
   stopButton->clear_visible_focus();

   startButton->callback((Fl_Callback *)startSimCB, this);
   pauseButton->callback((Fl_Callback *)pauseSimCB, this);
   stopButton->callback((Fl_Callback *)stopSimCB, this);

   startButton->activate();
   stopButton->deactivate();
   pauseButton->deactivate();

   assert(
      (startButton->active() && !stopButton->active() && !pauseButton->active()) ||
      (!startButton->active() && stopButton->active() && pauseButton->active()));
}

void ElevatorSimWindow::toggleButtons(ElevatorSimWindow* thisWin){
   static bool toggle = true; /* TODO: store this value somewhere else */ 

   assert(
      (thisWin->startButton->active() && 
      !thisWin->stopButton->active() && 
      !thisWin->pauseButton->active()) 
      ||
      (!thisWin->startButton->active() && 
      thisWin->stopButton->active() && 
      thisWin->pauseButton->active()));

   if(toggle){
      thisWin->startButton->deactivate();
      thisWin->stopButton->activate();
      thisWin->pauseButton->activate();
   } else {
      thisWin->stopButton->deactivate();
      thisWin->pauseButton->deactivate();
      thisWin->startButton->activate();
   }

   assert(
      (thisWin->startButton->active() && 
      !thisWin->stopButton->active() && 
      !thisWin->pauseButton->active()) 
      ||
      (!thisWin->startButton->active() && 
      thisWin->stopButton->active() && 
      thisWin->pauseButton->active()));

   toggle =! toggle;
}

void ElevatorSimWindow::buildDialogs() {
   /* Help Dialog */
   helpWin = new Fl_Window(300, 300, "Help");

   helpLabel = new Fl_Text_Display(10,30,280,190,"How to use:");
   helpDoneButton = new Fl_Button(100, 240, 100, 40, "Done");

   helpDoneButton->callback((Fl_Callback*) dismissHelpCB, this);

   helpWin->add(helpLabel);
   helpWin->add(helpDoneButton);
   helpWin->end();

   /* Confirmation dialog */
   confirmDialog = new Fl_Window(220, 110, "Are you sure?");
   yesButton = new Fl_Button(10, 10, 200, 40, "yes");
   noButton = new Fl_Button(10, 60, 200, 40, "no");
      
   yesButton->callback((Fl_Callback*) quitConfirmedCB, this);
   noButton->callback((Fl_Callback*) quitCancelledCB, this);

   confirmDialog->add(yesButton);
   confirmDialog->add(noButton);
   confirmDialog->end();
}

/* public static member initializers */
const char ElevatorSimWindow::WINDOW_TITLE[] = "elevatorSim";
const int ElevatorSimWindow::WINDOW_WIDTH = 640;
const int ElevatorSimWindow::WINDOW_HEIGHT = 480;
const int ElevatorSimWindow::MENUBAR_HEIGHT = 25;

/* public methods */
ElevatorSimWindow::ElevatorSimWindow(cTimeManager& _timeManager, cKeyManager& _keyManager) :
            Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE), timeManager(_timeManager), keyManager(_keyManager) {

   renderWindow = new ElevatorSimRenderWindow(
         keyManager,
         timeManager,
         ElevatorSimRenderWindow::LEFT_MARGIN,
         ElevatorSimRenderWindow::TOP_MARGIN,
         WINDOW_WIDTH - (ElevatorSimRenderWindow::LEFT_MARGIN + ElevatorSimRenderWindow::RIGHT_MARGIN),
         WINDOW_HEIGHT - (ElevatorSimRenderWindow::TOP_MARGIN + ElevatorSimRenderWindow::BOTTOM_MARGIN));

   resizable(*renderWindow);
   buildMenu();
   buildButtons();

   /* add more widgets to main window here */

   end();

   buildDialogs();

   callback((Fl_Callback*)windowCloseCB, this);

   /* add more callbacks to main window here */

   /* initialize any other main window member variables here */
}

ElevatorSimWindow::~ElevatorSimWindow() {
   delete confirmDialog;
   delete helpWin;
}

} /* namespace elevatorSim */

