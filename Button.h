#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include "Vector.h"
#include "Hitbox.h"

class Button {
private:
	double cx, cy, width, height;
	double lx, rx, ty, by;
	double texl, texr, text, texb;
	unsigned int texture;
	Vector color;
	double alpha;

public:
	Button();
	Button( double _cx, double _cy, double w, double h, unsigned int tex, double tl, double tr, double tt, double tb, Vector c );

	void display();
	bool testClicked( double mx, double my, int wWidth, int wHeight );

	void setText( double newVal );
	void setTexb( double newVal );
};

#endif