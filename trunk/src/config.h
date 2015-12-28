/*
*  This file centralize all includes and defines -
*/
#ifndef CONFIG_H
#define	CONFIG_H

#include <ctime>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <sstream>      // std::stringstream, std::stringbuf
using namespace std;

#include <cstring>      //necessario para: strlen()

//#include <GL/freeglut_std.h>
#include <GL/glut.h>

/** Mude conforme apropriado */
#ifdef _WIN32
 #include <windows.h>
#endif

// define para Windows
#ifdef __glut_h__
 #define HAVE_OPENGL
#endif

// define para Linux
#ifdef __GLUT_H__ 
 #define HAVE_OPENGL
#endif

/** Constantes inicialmente usadas pela classe GameData */
#define FULL_LIFE 3
#define FULL_TIME 5

#endif	/* CONFIG_H */
