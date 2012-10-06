#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

const float M_PI = 3.141592f;
const double FPS = 1/24;

struct Vec3f
{
	float x, y, z;
};

enum KEY_PRESS
{
	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_FORWARD,
	KEY_BACKWARD,
};

enum OBJ_LIST
{
	OBJ_CUBE = 1,
};

#endif //__DEFINES_H__