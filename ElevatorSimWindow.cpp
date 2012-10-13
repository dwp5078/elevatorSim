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

namespace elevatorSim {

   /* private methods */
   int ElevatorSimWindow::handle(int event) {
      switch(event) {
      case FL_KEYDOWN:
         switch(Fl::event_key()) {
         case FL_Up:
            renderWindow->m_vecCamPos.y += ElevatorSimRenderWindow::MOVE;
            renderWindow->m_vecCamLookAt.y += ElevatorSimRenderWindow::MOVE;
            return 1;

         case FL_Down:
            renderWindow->m_vecCamPos.y -= ElevatorSimRenderWindow::MOVE;
            renderWindow->m_vecCamLookAt.y -= ElevatorSimRenderWindow::MOVE;
            return 1;

         case FL_Right:
            renderWindow->m_vecCamPos.x += ElevatorSimRenderWindow::MOVE;
            renderWindow->m_vecCamLookAt.x += ElevatorSimRenderWindow::MOVE;
            return 1;

         case FL_Left:
            renderWindow->m_vecCamPos.x -= ElevatorSimRenderWindow::MOVE;
            renderWindow->m_vecCamLookAt.x -= ElevatorSimRenderWindow::MOVE;
            return 1;

         case FL_Page_Up:
            renderWindow->m_vecCamPos.z -= ElevatorSimRenderWindow::MOVE;
            return 1;

         case FL_Page_Down:
            renderWindow->m_vecCamPos.z += ElevatorSimRenderWindow::MOVE;
            return 1;
         }
      default:
         return Fl_Window::handle(event);
      }
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
      ElevatorSimWindow* thisWin = (ElevatorSimWindow*) userData;

      thisWin->showQuitConfirmDialog();
   }

   void ElevatorSimWindow::menuNewCB(Fl_Widget* w, void* userData) {
      /* TODO */
   }

   void ElevatorSimWindow::menuOpenCB(Fl_Widget* w, void* userData) {
      /* TODO */
   }

   void ElevatorSimWindow::menuSaveCB(Fl_Widget* w, void* userData) {
      /* TODO */
   }

   void ElevatorSimWindow::menuQuitCB(Fl_Widget* w, void* userData) {
      /* TODO */
   }

   void ElevatorSimWindow::menuAboutCB(Fl_Widget* w, void* userData) {
      /* TODO */
   }

   void ElevatorSimWindow::quitConfirmedCB(Fl_Button* yesButton, void* data) {
      ElevatorSimWindow* thisWin = (ElevatorSimWindow*) data;

      thisWin->hideQuitConfirmDialog();
      thisWin->hide();
   }

   void ElevatorSimWindow::quitCancelledCB(Fl_Button* noButton, void* data) {
      ElevatorSimWindow* thisWin = (ElevatorSimWindow*) data;

      thisWin->hideQuitConfirmDialog();
   }

   /* public static member initializers */
   const char ElevatorSimWindow::TITLE[] = "elevatorSim";
   const int ElevatorSimWindow::WIDTH = 640;
   const int ElevatorSimWindow::HEIGHT = 480;

   /* public methods */
   ElevatorSimWindow::ElevatorSimWindow() : Fl_Window(WIDTH, HEIGHT, TITLE) {
      renderWindow = new ElevatorSimRenderWindow(
         ElevatorSimRenderWindow::LEFT_MARGIN,
         ElevatorSimRenderWindow::TOP_MARGIN,
         WIDTH - (ElevatorSimRenderWindow::LEFT_MARGIN + ElevatorSimRenderWindow::RIGHT_MARGIN),
         HEIGHT - (ElevatorSimRenderWindow::TOP_MARGIN + ElevatorSimRenderWindow::BOTTOM_MARGIN));

      resizable(*renderWindow);

      Fl_Menu_Bar* menubar = new Fl_Menu_Bar(0, 0, w(), 25);

      menubar->add("&File/&New", FL_CTRL + 'n', menuNewCB, this);
      menubar->add("&File/&Open", FL_CTRL + 'o', menuOpenCB, this);
      menubar->add("&File/&Save", FL_CTRL + 's', menuSaveCB, this);
      menubar->add("&File/&Quit", FL_CTRL + 'q', menuQuitCB, this);
      menubar->add("&Help/&About", 0, menuAboutCB, this);

      add(menubar);
      end();

      callback((Fl_Callback*)windowCloseCB, this);

      confirmDialog = NULL;
      yesButton = NULL;
      noButton = NULL;
      wantedClose = false;
   }

} /* namespace elevatorSim */
