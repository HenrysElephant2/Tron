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

#define Cos(x) cos(3.1415926535/180*(x))
#define Sin(x) sin(3.1415926535/180*(x))

#define rSpeed .05
#define mSpeed .1


bool init(); //Starts up SDL, creates window, and initializes OpenGL
bool initGL(); //Initializes matrices and clear color
void keyDown( SDL_Keycode key, int x, int y ); //Input handler
void update(); //Per frame update
void render(); //Renders quad to the screen
void close(); //Frees media and shuts down SDL

static void Project();
void reshape( int width, int height );

int main( int argc, char* args[] );

#endif