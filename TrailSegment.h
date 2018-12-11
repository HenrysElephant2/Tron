#ifndef TRAILSEGMENT_H
#define TRAILSEGMENT_H

#include "Vector.h"
#include "Hitbox.h"
#include "LoadModel.h"
#include "TransparentRenderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define TRAIL_WIDTH .5
#define TRAIL_HEIGHT 10

#define FACE_L 0
#define FACE_T 1
#define FACE_R 2
#define FACE_B 3

class TrailSegment: public Transparent {
private:
	Vector *bb, *bt, *fb, *ft; //Back-bottom, back-top, front-bottom, front-top
	Hitbox *hitbox;
	TrailSegment *next;
	Vector color;
	unsigned int texture;

public:

	TrailSegment();
	TrailSegment( Vector *_bb, Vector *_bt, Vector *_fb, Vector *_ft, unsigned int t, Vector c = Vector(1,1,1) );
	~TrailSegment();

	// Accessors
	Vector* getBB();
	Vector* getBT();
	Vector* getFB();
	Vector* getFT();
	TrailSegment* getNext();
	Hitbox* getHitbox();
	Vector getColor();

	void setNext( TrailSegment *newNext );
	void setColor( Vector newColor );

	Vector getTopVector();
	Vector getBottomVector();
	void scaleToLengthF( double length, Vector tilt ); // Scale to length by changing front
	void scaleToLengthB( double length ); // Scale to length by changing back

	void updateFront( Vector *bPos, Vector *tPos );
	void updateBack( Vector *bPos, Vector *tPos );
	void calculateHitbox();

	bool testSegmentHit( Hitbox *other );

	void stage( TransparentRenderer *tr, Vector *cameraPos );
	void display( Vector *cameraPos );
	void displayFace( int face );
};

#endif