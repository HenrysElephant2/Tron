// Two player first-person game

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <iostream>
#include "LoadShader.h"
#include "GameState.h"
#include "Player.h"
#include "Map.h"
#include "Tile.h"
#include "Vector.h"
#include "Model.h"
#include "TransparentRenderer.h"


#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS
#define Cos(x) cos(3.1415926535/180*(x))
#define Sin(x) sin(3.1415926535/180*(x))
#endif

#define STATE_WAITING 0
#define STATE_PLAYING 1
#define STATE_P1_WIN 2
#define STATE_P2_WIN 3
#define STATE_TIE 4

#define MAP_WIDTH 8
#define MAP_LENGTH 8

#define NUMBER_GAUSSIAN_PASSES 16

class Gameplay: public GameState {
private:
	double ph;      //  Elevation of view angle
	double th;      //  Azimuth of view angle
	bool thup, thdown, phup, phdown; // Booleans to control window movement

	Player *player1, *player2;
	Vector p1color, p2color;
	Map *map;
	int state;


public:
	Gameplay();
	Gameplay( Vector p1c, Vector p2c );
	~Gameplay();

	// Inherited methods
	void keyDown(SDL_Keycode key, int x, int y); // need to format this specifically for SDL instead of GLUT
	void keyUp(SDL_Keycode key, int x, int y); // need to format this specifically for SDL instead of GLUT

	void mouseDown(int x, int y);
	void mouseUp(int x, int y);

	void special(int key, int x, int y); /*NEEDED in GLUT, maybe different in other system */
	void display();
	void update();

	void reset();
	void displayAll( Vector *cameraPos, bool renderBloom );
};

#endif






























