#include "Trail.h"

Trail::Trail() {
	end = NULL;
	start = NULL;
	length = 0;
}

Trail::~Trail() {
	if( end != NULL )
		delete end;
	if( start != NULL )
		delete start;
}

TrailSegment* Trail::getEnd() { return end; }
TrailSegment* Trail::getStart() { return start; }


void Trail::addSegment( Vector *newFB, Vector *newFT ) {
	TrailSegment *newSegment = new TrailSegment( start->getBB(), start->getBT(), newFB, newFT );
	start->setNext( newSegment );
	start = newSegment;
}

void Trail::removeSegment() {
	TrailSegment *newEnd = end->getNext();
	delete end;
	end = newEnd;
}


bool Trail::testTrailHit( Hitbox *other ) {
	return end->testSegmentHit( other );
}

void Trail::display() {
	end->display(); // Will loop from end to front displaying hitboxes
}