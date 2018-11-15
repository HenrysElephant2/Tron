#ifndef MAIN_H
#define MAIN_H

//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <string>
#include <iostream>
#include <stdio.h>

#include "Hitbox.h"
#include "Vector.h"
#include "Matrix.h"
#include "GameState.h"
#include "HitboxTest.h"

#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS
#define Cos(x) cos(3.1415926535/180*(x))
#define Sin(x) sin(3.1415926535/180*(x))
#endif


bool init(); //Starts up SDL, creates window, and initializes OpenGL
bool initGL(); //Initializes matrices and clear color
void close(); //Frees media and shuts down SDL
bool testQuit( SDL_Keycode key );

static void Project();
void reshape( int width, int height );

int main( int argc, char* args[] );

#endif