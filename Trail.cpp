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
	Vector curTilt = Add0( *newTop, Scale0( *newBottom, -1 ) );
	double diff = Vector( newBottom, start->getFB() ).getMagnitude();

	start->updateFront( newBottom, newTop );
	if( start->getBottomVector().getMagnitude() > MAX_SEGMENT_LENGTH ) {
		start->scaleToLengthF(MAX_SEGMENT_LENGTH, curTilt);

		Vector modVec = Add0( *newBottom, Scale0( *(start->getFB()), -1 ) );
		double toAdd = modVec.getMagnitude();
		modVec.Normalize(); modVec.Scale( MAX_SEGMENT_LENGTH );

		while( toAdd > MAX_SEGMENT_LENGTH ) {
			Vector* curBottom = new Vector();
			*curBottom = Add0( *(start->getFB()), modVec );

			Vector* curTop = new Vector();
			*curTop = Add0( *(start->getFT()), modVec );

			addSegment( curBottom, curTop );

			toAdd -= MAX_SEGMENT_LENGTH;
		}
		addSegment( newBottom, newTop );
	}

	if( limit && length > MAX_TRAIL_SEGMENTS ) {
		while( length > MAX_TRAIL_SEGMENTS ) {
			removeSegment();
			diff -= MAX_SEGMENT_LENGTH;
		}
		end->scaleToLengthB( MAX_SEGMENT_LENGTH - diff );
	}
}


void Trail::addSegment( Vector *newFB, Vector *newFT ) {
	Vector *newBB = new Vector(); newBB->set(start->getFB());
	Vector *newBT = new Vector(); newBT->set(start->getFT());
	TrailSegment *newSegment = new TrailSegment( newBB, newBT, newFB, newFT );
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