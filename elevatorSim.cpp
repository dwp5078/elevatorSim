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
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>

const static int    ELEVATOR_SIM_WINDOW_WIDTH = 640;
const static int    ELEVATOR_SIM_WINDOW_HEIGHT = 480;
const static int    ELEVATOR_SIM_MARGIN = 20;
const static char   ELEVATOR_SIM_TITLE[] = "elevatorSim";
const static char   ELEVATOR_SIM_VERSION[] = "v0.0.0-pre";

int main(int argc, char** argv) {
   Fl_Window *win = new Fl_Window(
      ELEVATOR_SIM_WINDOW_WIDTH,
      ELEVATOR_SIM_WINDOW_HEIGHT,
      ELEVATOR_SIM_TITLE);

   Fl_Text_Display *disp = new Fl_Text_Display(
      ELEVATOR_SIM_MARGIN,
      ELEVATOR_SIM_MARGIN,
      ELEVATOR_SIM_WINDOW_WIDTH - (2 * ELEVATOR_SIM_MARGIN),
      ELEVATOR_SIM_WINDOW_HEIGHT - (2 * ELEVATOR_SIM_MARGIN));

   Fl_Text_Buffer *tbuff = new Fl_Text_Buffer();

   disp->buffer(tbuff);
   tbuff->text(ELEVATOR_SIM_VERSION);
   win->resizable(*disp);
   win->show();
   
   return(Fl::run());
}
