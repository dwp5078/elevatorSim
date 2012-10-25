/*
* Copyright (c) 2012, Joseph Max DeLiso, Daniel Gilbert
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

#ifndef _ELEVATOR_SIM_WINDOW_H
#define _ELEVATOR_SIM_WINDOW_H

#include "ElevatorSim.hpp"
#include "ElevatorSimRenderWindow.hpp"
#include "cTimeManager.hpp"
#include "cKeyManager.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Bar.H>

namespace elevatorSim {

   class ElevatorSimWindow : public Fl_Window {

      /* private methods */
      int handle(int event);
      void showQuitConfirmDialog();
      void hideQuitConfirmDialog();
      void buildMenu();
      void buildButtons();
      void openScript();

      /* private static methods */
      static void windowCloseCB(Fl_Window* w, void* userData);
      static void menuNewCB(Fl_Widget* w, void* userData);
      static void menuOpenCB(Fl_Widget* w, void* userData);
      static void menuSaveCB(Fl_Widget* w, void* userData);
      static void menuQuitCB(Fl_Widget* w, void* userData);
      static void menuAboutCB(Fl_Widget* w, void* userData);
      static void menuHelpCB(Fl_Widget* w, void* userData);
      static void startSimCB(Fl_Widget* w, void* userData);
      static void pauseSimCB(Fl_Widget* w, void* userData);
      static void stopSimCB(Fl_Widget* w, void* userData);
      static void quitConfirmedCB(Fl_Button* yesButton, void* data);
      static void quitCancelledCB(Fl_Button* noButton, void* data);
      static void dismissHelpCB(Fl_Widget* w, void* userData);

      /* private members */
      ElevatorSimRenderWindow* renderWindow;
      Fl_Window* confirmDialog;
      Fl_Button* yesButton;
      Fl_Button* noButton;
      cTimeManager& timeManager;
      cKeyManager& keyManager;

   public:

      /* public static members */
      const static int WIDTH;
      const static int HEIGHT;
      const static char TITLE[];

      /* public methods */
      ElevatorSimWindow(cTimeManager& _timeManager, cKeyManager& _keyManager);
   };

} /* namespace elevatorSim */

#endif

