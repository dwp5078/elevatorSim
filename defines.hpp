#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <cassert>
#include <cmath>

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

const GLfloat light1_ambient[]	= { 0.4f,	0.4f,	0.4f,	1.0f };
const GLfloat light1_diffuse[]	= { 0.8f,	0.8f,	0.8f,	1.0f };
const GLfloat light1_specular[]	= { 0.5f,	0.5f,	0.5f,	1.0f };
const GLfloat light1_position[]	= { 3.0f,	10.0f,	3.0f,	0.0f };
const GLfloat light1_direction[]	= { 0.0f, 0.0f, 0.0f, 0.0f };

#endif //__DEFINES_H__