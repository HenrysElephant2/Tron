#ifndef TRAILSEGMENT_H
#define TRAILSEGMENT_H

#include "Vector.h"
#include "Hitbox.h"

#define TRAIL_WIDTH .5
#define TRAIL_HEIGHT 8

class TrailSegment {
private:
	Vector *bb, *bt, *fb, *ft; //Back-bottom, back-top, front-bottom, front-top
	Hitbox *hitbox;
	TrailSegment *next;

public:
	TrailSegment();
	TrailSegment( Vector *_bb, Vector *_bt, Vector *_fb, Vector *_ft );
	~TrailSegment();

	// Accessors
	Vector* getBB();
	Vector* getBT();
	Vector* getFB();
	Vector* getFT();
	TrailSegment* getNext();

	void setNext( TrailSegment *newNext );

	void updateFront( Vector *bPos, Vector *tPos );
	void updateBack( Vector *bPos, Vector *tPos );
	void calculateHitbox();

	bool testSegmentHit( Hitbox *other );

	void display();
};

#endif