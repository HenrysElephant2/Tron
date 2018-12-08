#ifndef TRAIL_H
#define TRAIL_H


#define MAX_TRAIL_SEGMENTS 300 // 20 meters
#define MAX_SEGMENT_LENGTH 3 // decimeter

#include "Vector.h"
#include "Hitbox.h"
#include "TrailSegment.h"
#include "TransparentRenderer.h"
#include <iostream>

class Trail {
private:
	TrailSegment *end, *start; // Trail linked list will be ordered from back to front, so end is actually the first segment
	int length;
	bool limit;
	unsigned int texture;
	Vector color;

public:
	Trail();
	Trail( Vector *startBottom, Vector *startTop, bool l, unsigned int t, Vector c = Vector(1,1,1) );
	~Trail();

	TrailSegment* getEnd();
	TrailSegment* getStart();
 	
	void update( Vector *newBottom, Vector *newTop, Vector newColor );

	void addSegment( Vector *newFB, Vector *newFT, Vector newColor );
	void removeSegment();

	bool testTrailHit( Hitbox* other );

	void stage( Vector trailEnd, Vector *tilt, TransparentRenderer *tr, Vector *cameraPos, Vector newColor );
};

#endif