#ifndef HITBOX_H
#define HITBOX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Vector.h"
#include "Matrix.h"
#include <iostream>

// Position Indicators : Top/Bottom - Front/Back - Left/Right
#define BBL 0
#define BBR 1
#define BFR 2
#define BFL 3
#define TBL 4
#define TBR 5
#define TFR 6
#define TFL 7


class Hitbox {
private:
	Vector *pos, *forward, *up, *cross;
	double width, length, height; // width: side/side, length: forward/back

	Vector *points[8]; // To calculate in constructor

	void calculatePoints();

public:
	Hitbox();
	Hitbox( Vector *p, Vector *f, Vector *u, double w, double l, double h );
	~Hitbox();

	void updateVecs( Vector *newPos, Vector *newForward, Vector *newUp, bool calc = true );
	void updateDimensions( double newW, double newL, double newH, bool calc = true );

	Vector* getPos();
	Vector* getForward();
	Vector* getUp();
	double getWidth();
	double getLength();
	double getHeight();
	Vector* getPoint(int i);

	void setPos( Vector *toSet );
	void setForward( Vector *toSet );
	void setUp( Vector *toSet );
	void setWidth( double toSet );
	void setLength( double toSet );
	void setHeight( double toSet );

	void move( Vector *dv );

	friend bool testCollision( Hitbox *h1, Hitbox *h2 );
};

#endif
