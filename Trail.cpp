#include "Trail.h"

Trail::Trail(){}

Trail::Trail( Vector *startBottom, Vector *startTop, bool l ) {
	end = new TrailSegment( startBottom, startTop, startBottom, startTop );
	start = end;
	length = 0;
	limit = l;
}

Trail::~Trail() {
	if( end != NULL )
		delete end;
	if( start != NULL && start != end  )
		delete start;
}

TrailSegment* Trail::getEnd() { return end; }
TrailSegment* Trail::getStart() { return start; }


void Trail::update( Vector *newBottom, Vector *newTop ) {
	double diff = Vector( newBottom, start->getFB() ).getMagnitude();
	start->updateFront( newBottom, newTop );
	if( start->getBottomVector().getMagnitude() > MAX_SEGMENT_LENGTH ) {
		start->scaleToLengthF(MAX_SEGMENT_LENGTH);
		addSegment( newBottom, newTop );
	}

	if( limit && length > MAX_TRAIL_SEGMENTS ) {
		while( length > MAX_TRAIL_SEGMENTS ) {
			removeSegment();
		}
		end->scaleToLengthB( MAX_SEGMENT_LENGTH - diff );
	}
}


void Trail::addSegment( Vector *newFB, Vector *newFT ) {
	TrailSegment *newSegment = new TrailSegment( start->getFB(), start->getFT(), newFB, newFT );
	start->setNext( newSegment );
	start = newSegment;
	length++;
}

void Trail::removeSegment() {
	TrailSegment *newEnd = end->getNext();
	delete end;
	end = newEnd;
	length--;
}


bool Trail::testTrailHit( Hitbox *other ) {
	return end == NULL ? false : end->testSegmentHit( other );
}

void Trail::display() {
	end == NULL ? ((void)0) : end->display(); // Will loop from end to front displaying hitboxes
}