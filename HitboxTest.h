#ifndef HITBOXTEST_H
#define HITBOXTEST_H

#include "GameState.h"
#include "Hitbox.h"

#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS
#define Cos(x) cos(3.1415926535/180*(x))
#define Sin(x) sin(3.1415926535/180*(x))
#endif

#define rSpeed .05
#define mSpeed .1

class HitboxTest: public GameState {
private:
	Hitbox *h1, *h2;

	double ph;      //  Elevation of view angle
	double th;      //  Azimuth of view angle
	bool thup, thdown, phup, phdown; // Booleans to control window movement
	bool hup, hdown, hleft, hright, hforward, hbackward; // Bools to move hitbox
	bool pu, pd, yl, yr, rr, rl; // Bools to rotate hitbox

	bool colliding;

public:
	HitboxTest();
	~HitboxTest();

	void keyDown(SDL_Keycode key, int x, int y); // need to format this specifically for SDL instead of GLUT
	void keyUp(SDL_Keycode key, int x, int y); // need to format this specifically for SDL instead of GLUT
	
	void special(int key, int x, int y); /*NEEDED in GLUT, maybe different in other system */
	void mouse(/*TODO*/);

	void display();
	void update();
};

#endif