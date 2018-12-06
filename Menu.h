#ifndef MENU_H
#define MENU_H

#include <iostream>
#include "LoadShader.h"
#include "GameState.h"
#include "Player.h"
#include "Map.h"
#include "Tile.h"
#include "Vector.h"
#include "Model.h"
#include "TransparentRenderer.h"
#include "Gameplay.h"
#include "Button.h"

#define MAIN_MENU 0
#define COLOR_SELECT 1

#define NONE_DOWN 0
#define PLAY_DOWN 1
#define COLORS_DOWN 2
#define BACK_DOWN 3

#define NCOLORS 9
#define BUTTON(p,i) (p*NCOLORS+i)

class Menu: public GameState {
private:
	int state, mouseState;

	double ph;      //  Elevation of view angle
	double th;      //  Azimuth of view angle

	Player *player1, *player2;
	int p1color, p2color;
	Map *map;

	Vector menuButtonColor = Vector(.5,.8,1);

	// Main menu buttons
	Button tronLogo, playButton, colorsButton;

	// Color select buttons
	Button backButton;
	Button p1cButtons[NCOLORS];
	Button p2cButtons[NCOLORS];

	Vector colors[NCOLORS] = {
		Vector(.9,.9,1), //White
		Vector(1,0,0), //Red
		Vector(1,.3,0), //Orange
		Vector(1,.8,0), //Yellow
		Vector(.2,1,0), //Green
		Vector(0,1,.5), //Aqua marine
		Vector(0,.8,1), //Blue
		Vector(.5,0,1), //Purple
		Vector(1,0,1) //Pink
	};

public:
	Menu();
	~Menu();

	// Inherited methods
	void keyDown(SDL_Keycode key, int x, int y); // need to format this specifically for SDL instead of GLUT
	void keyUp(SDL_Keycode key, int x, int y); // need to format this specifically for SDL instead of GLUT

	void mouseDown(int x, int y);
	void mouseUp(int x, int y);

	void special(int key, int x, int y); /*NEEDED in GLUT, maybe different in other system */
	void display();
	void update();

	void displayAll( Vector *cameraPos, Vector *targetLoc, bool renderBloom );
};

#endif