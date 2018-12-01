#ifndef T_RENDERER_H
#define T_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

#include "Vector.h"

class TransparentRenderer; 

class Transparent {
public:
	virtual ~Transparent() = 0;
	virtual void stage( TransparentRenderer *tr, Vector *cameraPos ) = 0;
	virtual void display( Vector *cameraPos ) = 0;
};

class TLL {
public:
	Transparent *ob;
	TLL *next;
	double dist;

	TLL( Transparent *o, double d );
	~TLL();
};

class TransparentRenderer {
private:
	TLL *start, *end;
	int num;

public:
	TransparentRenderer();
	~TransparentRenderer();

	void add( Transparent *toAdd, double dist );
	void display( Vector *cameraPos );

	void Merge( Transparent *obs[], double dists[], int low, int high, int mid );
	void MergeSort( Transparent *obs[], double dists[], int low, int high );
};

#endif