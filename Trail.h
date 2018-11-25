#ifndef TRAIL_H
#define TRAIL_H


#define MAX_TRAIL_SEGMENTS 50 // 20 meters
#define MAX_SEGMENT_LENGTH 5 // decimeter

#include "Vector.h"
#include "Hitbox.h"
#include "TrailSegment.h"
#include <iostream>

class Trail {
private:
	TrailSegment *end, *start; // Trail linked list will be ordered from back to front, so end is actually the first segment
	int length;
	bool limit;

public:
	Trail();
	Trail( Vector *startBottom, Vector *startTop, bool l );
	~Trail();

	TrailSegment* getEnd();
	TrailSegment* getStart();
 	
	void update( Vector *newBottom, Vector *newTop );

	void addSegment( Vector *newFB, Vector *newFT );
	void removeSegment();

	bool testTrailHit( Hitbox* other );

	void display();
};

#endif