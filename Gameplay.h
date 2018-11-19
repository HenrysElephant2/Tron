// Two player first-person game

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "GameState.h"
#include "Player.h"
#include "Map.h"
#include "Vector.h"

#define STATE_WAITING 0
#define STATE_PLAYING 1
#define STATE_P1_WIN 2
#define STATE_P2_WIN 3
#define STATE_TIE 4

class Gameplay: public GameState {
	Player *player1, *player2;
	Map *map;
	int state;

	Gameplay();
	~Gameplay();

	// Inherited methods
	void keyDown(SDL_Keycode key, int x, int y); // need to format this specifically for SDL instead of GLUT
	void keyUp(SDL_Keycode key, int x, int y); // need to format this specifically for SDL instead of GLUT

	void special(int key, int x, int y); /*NEEDED in GLUT, maybe different in other system */
	void mouse(/*TODO*/);
	void display();
	void update();
};

#endif