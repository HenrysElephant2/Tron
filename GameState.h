#ifndef GAMESTATE_H
#define GAMESTATE_H

//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class GameState {
public:
	virtual void keyDown(SDL_Keycode key, int x, int y) = 0; // need to format this specifically for SDL instead of GLUT
	virtual void keyUp(SDL_Keycode key, int x, int y) = 0; // need to format this specifically for SDL instead of GLUT

	virtual void special(int key, int x, int y) = 0; /*NEEDED in GLUT, maybe different in other system */
	virtual void mouse(/*TODO*/) = 0;
	virtual void display() = 0;
	virtual void update() = 0;
};

#endif