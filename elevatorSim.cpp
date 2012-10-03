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

#if defined(_ES_WINNT)
#include <Windows.h>
#elif defined(_ES_DARWIN) || defined(_ES_UNIX)
#include <unistd.h>
#else
#error Unspecified operating system. Use the makefile.
#endif

#include <iostream>
#include <boost/thread.hpp>
#include <sqlite/sqlite3.h>

#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Gl_Window.H>

class ElevatorRenderWindow : public Fl_Gl_Window {

   void draw();
   int handle(int event);

   public:

   const static int LEFT_MARGIN;
   const static int RIGHT_MARGIN;
   const static int TOP_MARGIN;
   const static int BOTTOM_MARGIN;

   ElevatorRenderWindow(int X, int Y, int W, int H, const char* Label = 0);
};

const int ElevatorRenderWindow::LEFT_MARGIN = 120;
const int ElevatorRenderWindow::RIGHT_MARGIN = 8;
const int ElevatorRenderWindow::TOP_MARGIN = 8;
const int ElevatorRenderWindow::BOTTOM_MARGIN = 8;

ElevatorRenderWindow::ElevatorRenderWindow(int X, int Y, int W, int H, const char* Label) :
   Fl_Gl_Window(X, Y, W, H, Label) {

}

void ElevatorRenderWindow::draw() {
   if(!valid()) {
      /* initialize */
   }

   /* draw */
}

int ElevatorRenderWindow::handle(int event) {
   switch(event) {
      case FL_FOCUS:

      return 1;

      case FL_UNFOCUS:

      return 1;

      /* ... handle more events ... */

      default:
         return Fl_Gl_Window::handle(event);
   }
}

class ElevatorSimWindow : public Fl_Window {

   public:

   const static int WIDTH;
   const static int HEIGHT;
   const static char TITLE[];

   ElevatorSimWindow();
};

ElevatorSimWindow::ElevatorSimWindow() : Fl_Window(WIDTH, HEIGHT, TITLE) {
   ElevatorRenderWindow *erw = new ElevatorRenderWindow(
   ElevatorRenderWindow::LEFT_MARGIN,
   ElevatorRenderWindow::TOP_MARGIN,
   WIDTH - (ElevatorRenderWindow::LEFT_MARGIN + ElevatorRenderWindow::RIGHT_MARGIN),
   HEIGHT - (ElevatorRenderWindow::TOP_MARGIN + ElevatorRenderWindow::BOTTOM_MARGIN));

   resizable(*erw);
   end();
}

const char ElevatorSimWindow::TITLE[] = "elevatorSim";
const int ElevatorSimWindow::WIDTH = 640;
const int ElevatorSimWindow::HEIGHT = 480;

int main(int argc, char** argv) {
   ElevatorSimWindow *mainWin = new ElevatorSimWindow();
   mainWin->show();
   return(Fl::run());
}
