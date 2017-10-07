////////////////////////////////////////
// core.h
////////////////////////////////////////

#ifndef CSE169_CORE_H
#define CSE169_CORE_H

////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cstring>
#include <ctype.h>
#include <cstdlib>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include <windows.h>
#define M_PI	3.14159f
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>
#endif

using namespace std;

static const int MAX_DOF = 1000;
static const int MIN_DOF = -1000;

void drawAxis(float size);
void drawWireBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax, bool mode);

////////////////////////////////////////////////////////////////////////////////

/*
This file just has a bunch of common stuff used by all objects. It mainly just
includes GL and some other standard headers.
*/

#endif
