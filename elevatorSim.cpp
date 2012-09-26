#include <Windows.h> 
/* 
 * #include <unistd.h> 
 * TODO: switch on this with macro
 */
 
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
