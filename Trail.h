#ifndef TRAIL_H
#define TRAIL_H

#define MAX_TRAIL_SEGMENTS 100

#include "Vector.h"
#include "Hitbox.h"
#include "TrailSegment.h"

class Trail {
private:
	TrailSegment *end, *start; // Trail linked list will be ordered from back to front, so end is actually the first segment
	int length;

public:
	Trail();
	~Trail();

	TrailSegment* getEnd();
	TrailSegment* getStart();

	void addSegment( Vector *newFB, Vector *newFT );
	void removeSegment();

	bool testTrailHit( Hitbox* other );
};

#endif