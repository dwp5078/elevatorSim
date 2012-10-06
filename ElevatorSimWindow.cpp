/*
* Copyright (c) 2012, Joseph Max DeLiso
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
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

void quit_cb(Fl_Widget*, void*) {}

/*Fl_Menu_Item menutable[] = {
  {"&File",0,0,0,FL_SUBMENU},
    {"&Open",   FL_ALT+'o', 0, 0, FL_MENU_INACTIVE},
    {"&Close",  0, 0},
    {"&Quit",   FL_ALT+'q', quit_cb, 0, FL_MENU_DIVIDER},
    {0},
  {"&Edit",FL_F+2,0,0,FL_SUBMENU},
    {"Undo",    FL_ALT+'z',     0},
    {"Redo",    FL_ALT+'r',     0, 0, FL_MENU_DIVIDER},
    {"Cut",     FL_ALT+'x',     0},
    {"Copy",    FL_ALT+'c',     0},
    {"Paste",   FL_ALT+'v',     0},
    {"Inactive",FL_ALT+'d',     0, 0, FL_MENU_INACTIVE},
    {"Clear",   0,      0, 0, FL_MENU_DIVIDER},
    {"Invisible",FL_ALT+'e',    0, 0, FL_MENU_INVISIBLE},
    {"Preferences",0,   0},
    {"Size",    0,      0},
    {0},
  {0}
};*/

namespace elevatorSim {

   int ElevatorSimWindow::handle(int event) {
      switch(event) {

      case FL_KEYDOWN:

         if(Fl::event_key() == FL_Up) renderWindow->key_pressed(KEY_UP);
         else if(Fl::event_key() == FL_Down) renderWindow->key_pressed(KEY_DOWN);
         else if(Fl::event_key() == FL_Left) renderWindow->key_pressed(KEY_LEFT);
         else if(Fl::event_key() == FL_Right) renderWindow->key_pressed(KEY_RIGHT);
         else if(Fl::event_key() == FL_Page_Up) renderWindow->key_pressed(KEY_FORWARD);
         else if(Fl::event_key() == FL_Page_Down) renderWindow->key_pressed(KEY_BACKWARD);
         else if(Fl::event_key() == FL_Escape)  return Fl_Window::handle(event);
         return 1;


      case FL_HIDE:
         if(wantedClose) {
            return Fl_Window::handle(event);
         } else {
            show();
            showQuitConfirmDialog();
            return 1;
         }

      default:
         return Fl_Window::handle(event);
      }
   }

   void quitConfirmedCB(Fl_Button* yesButton, void* data) {
      ElevatorSimWindow* thisWin = (ElevatorSimWindow*) data;

      thisWin->wantedClose = true;
      thisWin->hideQuitConfirmDialog();
      thisWin->hide();
   }

   void quitCancelledCB(Fl_Button* noButton, void* data) {
      ElevatorSimWindow* thisWin = (ElevatorSimWindow*) data;
      thisWin->wantedClose = false;
      thisWin->hideQuitConfirmDialog();
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

   ElevatorSimWindow::ElevatorSimWindow() : Fl_Window(WIDTH, HEIGHT, TITLE) {
      

      renderWindow = new ElevatorSimRenderWindow(
         ElevatorSimRenderWindow::LEFT_MARGIN,
         ElevatorSimRenderWindow::TOP_MARGIN,
         WIDTH - (ElevatorSimRenderWindow::LEFT_MARGIN + ElevatorSimRenderWindow::RIGHT_MARGIN),
         HEIGHT - (ElevatorSimRenderWindow::TOP_MARGIN + ElevatorSimRenderWindow::BOTTOM_MARGIN));

      resizable(*renderWindow);

      Fl_Menu_Bar* menubar = new Fl_Menu_Bar(0, 0, w(), 25);
      
      //menubar.menu(menutable);
      menubar->add("&File/&New", FL_CTRL + 'n', Menu_CB_New);
      menubar->add("&File/&Open", FL_CTRL + 'o', Menu_CB_Open);
      menubar->add("&File/&Save", FL_CTRL + 's', Menu_CB_Save);
      menubar->add("&File/&Quit", FL_CTRL + 'q', Menu_CB_Quit);

      menubar->add("&Edit/&Paste", FL_CTRL + 'v', Menu_CB_Quit);
      menubar->add("&Help/&About", 0, Menu_CB_Quit);
      
      add(menubar);

      end();

      confirmDialog = NULL;
      yesButton = NULL;
      noButton = NULL;
      wantedClose = false;
   }

   const char ElevatorSimWindow::TITLE[] = "elevatorSim";
   const int ElevatorSimWindow::WIDTH = 640;
   const int ElevatorSimWindow::HEIGHT = 480;

} /* namespace elevatorSim */