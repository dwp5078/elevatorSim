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
#include <FL/names.h>

namespace elevatorSim {

/* private methods */
int ElevatorSimWindow::handle(int event) {
   int lastKey = Fl::event_key();

   if(isDebugBuild()) {
      printf("MainWin: event: %s (%d)\n", fl_eventnames[event], event);
   }

   if(event == FL_KEYDOWN)  {
      keyManager.down(lastKey);
      return true;
   } else if ( event == FL_KEYUP) {
      keyManager.up(lastKey);
      return true;
   }

   return Fl_Window::handle(event);
}

void ElevatorSimWindow::showQuitConfirmDialog() {
   /* Lazy-allocation of quit dialog widgets */
   if(!confirmDialog && !yesButton && !noButton) {
      confirmDialog = new Fl_Window(220, 110, "Are you sure?");
      yesButton = new Fl_Button(10, 10, 200, 40, "yes");
      noButton = new Fl_Button(10, 60, 200, 40, "no");

      yesButton->callback((Fl_Callback*) quitConfirmedCB, this);
      noButton->callback((Fl_Callback*) quitCancelledCB, this);

      confirmDialog->add(yesButton);
      confirmDialog->add(noButton);
      confirmDialog->end();
   }

   confirmDialog->show();
}

void ElevatorSimWindow::hideQuitConfirmDialog() {
   if(confirmDialog) {
      confirmDialog->hide();
   }
}

/* private static methods */
void ElevatorSimWindow::windowCloseCB(Fl_Window* w, void* userData) {
   if(isDebugBuild()) {
      printf("windowCloseCB fired with widget ptr @%p\n", w);
   }

   ElevatorSimWindow* thisWin = (ElevatorSimWindow*) userData;
   thisWin->showQuitConfirmDialog();
}

void ElevatorSimWindow::menuNewCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      printf("menuNewCB fired with widget ptr @%p and userData @%p\n", w, userData);
   }

   /* TODO */
}

void ElevatorSimWindow::menuOpenCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      printf("menuOpenCB fired with widget ptr @%p\n", w);
   }

   ElevatorSimWindow* thisWin = (ElevatorSimWindow*) userData;
   thisWin->openScript();
}

void ElevatorSimWindow::menuHelpCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      printf("menuHelpCB fired with widget ptr @%p\n", w);
   }

   ElevatorSimWindow* thisWindow = (ElevatorSimWindow*) userData;

   if(!thisWindow->helpWin && !thisWindow->helpLabel && !thisWindow->helpDoneButton) {
      thisWindow->helpWin = new Fl_Window(300, 300, "Help");
      thisWindow->helpLabel = new Fl_Text_Display(10,30,280,190,"How to use:");
      thisWindow->helpDoneButton = new Fl_Button(100, 240, 100, 40, "Done");

      thisWindow->helpDoneButton->callback((Fl_Callback*) dismissHelpCB, userData);

      thisWindow->helpWin->add(thisWindow->helpLabel);
      thisWindow->helpWin->add(thisWindow->helpDoneButton);
      thisWindow->helpWin->end();
   }

   thisWindow->helpWin->show();
}

void ElevatorSimWindow::dismissHelpCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      printf("dismissHelpCB fired with widget ptr @%p\n", w);
   }

   ElevatorSimWindow* thisWindow = (ElevatorSimWindow*) userData;
   thisWindow->helpWin->hide();
}

void ElevatorSimWindow::startSimCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      printf("in startSimCB with widget ptr @%p and userData @%p\n", w, userData);
   }

   Fl_Button* startButton = (Fl_Button*)w;

   if(startButton->value()) {
      if(isDebugBuild() ) {
         printf("startSim CB fired\n");
      }

      /* TODO */
   }
}

void ElevatorSimWindow::pauseSimCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      printf("in pauseSimCB with widget ptr @%p and userData @%p\n", w, userData);
   }

   Fl_Button* pauseButton = (Fl_Button*)w;
   /* TODO: this needs to be moved into time manager */
   static bool paused = true;

   if(pauseButton->value()) {
      printf("pauseSim CB fired\n");

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
      printf("in stopSimCB with widget ptr @%p and userData @%p\n", w, userData);
   }

   Fl_Button* stopButton = (Fl_Button*)w;

   if(stopButton->value()) {
      printf("stopSim CB fired\n");
   }
}

void ElevatorSimWindow::menuSaveCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      printf("menuSaveCB fired with widget ptr @%p and userData @%p\n", w, userData);
   }

   /* TODO */
}

void ElevatorSimWindow::menuQuitCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      printf("menuQuitCB fired with widget ptr @%p\n", w);
   }

   ElevatorSimWindow* thisWin = (ElevatorSimWindow*) userData;
   thisWin->showQuitConfirmDialog();
}

void ElevatorSimWindow::menuAboutCB(Fl_Widget* w, void* userData) {
   if(isDebugBuild()) {
      printf("menuAboutCB fired with widget ptr @%p and userData @%p\n", w, userData);
   }

   /* no - windows api calls are disallowed
      ShellExecute(NULL, "open", "https://github.com/maxdeliso/elevatorSim",
      NULL, NULL, SW_SHOWNORMAL);*/
}

void ElevatorSimWindow::quitConfirmedCB(Fl_Button* yesButton, void* userData) {
   if(isDebugBuild()) {
      printf("quitConfirmedCB fired with button widget ptr @%p\n", yesButton);
   }

   ElevatorSimWindow* thisWin = (ElevatorSimWindow*) userData;

   thisWin->hideQuitConfirmDialog();
   thisWin->hide();
}

void ElevatorSimWindow::quitCancelledCB(Fl_Button* noButton, void* userData) {
   if(isDebugBuild()) {
      printf("quitCancelledCB fired with button widget ptr @%p\n", noButton);
   }

   ElevatorSimWindow* thisWin = (ElevatorSimWindow*) userData;

   thisWin->hideQuitConfirmDialog();
}

void ElevatorSimWindow::openScript() {
   /* fl_file_chooser("Open Python Script", "*.py", "", 0); */
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
         { "E&xit", FL_COMMAND + 'q', (Fl_Callback *)menuQuitCB, this },
         { 0 },
         { "&Help", 0, 0, 0, FL_SUBMENU },
         { "How to use", 0, (Fl_Callback *)menuHelpCB, this },
         { "Website", 0, (Fl_Callback *)menuAboutCB, this },
         { 0 },
         { 0 }};

   Fl_Menu_Bar* menubar = new Fl_Menu_Bar(0, 0, w(), 25);
   menubar->copy(menuitems);

   add(menubar);
}

void ElevatorSimWindow::buildButtons(){
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
}

/* public static member initializers */
const char ElevatorSimWindow::TITLE[] = "elevatorSim";
const int ElevatorSimWindow::WIDTH = 640;
const int ElevatorSimWindow::HEIGHT = 480;

/* public methods */
ElevatorSimWindow::ElevatorSimWindow(cTimeManager& _timeManager, cKeyManager& _keyManager) :
            Fl_Window(WIDTH, HEIGHT, TITLE), timeManager(_timeManager), keyManager(_keyManager) {

   renderWindow = new ElevatorSimRenderWindow(
         keyManager,
         timeManager,
         ElevatorSimRenderWindow::LEFT_MARGIN,
         ElevatorSimRenderWindow::TOP_MARGIN,
         WIDTH - (ElevatorSimRenderWindow::LEFT_MARGIN + ElevatorSimRenderWindow::RIGHT_MARGIN),
         HEIGHT - (ElevatorSimRenderWindow::TOP_MARGIN + ElevatorSimRenderWindow::BOTTOM_MARGIN));

   resizable(*renderWindow);
   buildMenu();
   buildButtons();

   /* add more widgets to main window here */

   end();

   callback((Fl_Callback*)windowCloseCB, this);

   /* add more callbacks to main window here */

   confirmDialog = NULL;
   yesButton = NULL;
   noButton = NULL;

   helpWin = NULL;
   helpLabel = NULL;
   helpDoneButton = NULL;

   /* initialize any other main window member variables here */
}

} /* namespace elevatorSim */

