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

#ifndef _ELEVATOR_SIM_H
#define _ELEVATOR_SIM_H

#if defined(_ES_WINNT)
#include <Windows.h>
#elif defined(_ES_DARWIN) || defined(_ES_UNIX)
#include <unistd.h>
#else
#error Unspecified operating system. Use the makefile.
#endif

/*
* #include <iostream>
* #include <boost/thread.hpp>
* #include <sqlite/sqlite3.h>
*/

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include <cassert>
#include <cmath>

#include "util_vectors.hpp"

namespace elevatorSim {

const float M_PI = 3.141592f;
const double FPS = 1/(double)60;

const int g_nNumberOfFloor = 10;
const int g_nNumberOfElev = 5;

const float ELEV_GAP_WIDTH   = 1.7f;
const float BUILDING_GAP_HEIGHT   = 1.1f;

enum KEY_PRESS {
   KEY_UP,
   KEY_DOWN,
   KEY_RIGHT,
   KEY_LEFT,
   KEY_FORWARD,
   KEY_BACKWARD,
};

const GLfloat light1_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
const GLfloat light1_diffuse[] = { 0.15f, 0.15f, 0.15f, 1.0f };
const GLfloat light1_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat light1_position[] = { 1.f, 8.f, 2.0f, 0.0f };
const GLfloat light1_direction[] = { 0.0f, 0.0f, 0.0f, 0.0f };

}

#endif
