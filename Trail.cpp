#include "Trail.h"

Trail::Trail(){}

Trail::Trail( Vector *startBottom, Vector *startTop, bool l, unsigned int t, Vector c ) {
	end = new TrailSegment( startBottom, startTop, startBottom, startTop, t, c );
	start = end;
	length = 0;
	limit = l;
	texture = t;
	color = c;
}

Trail::~Trail() {
	TrailSegment *cur = end;
	while( cur != NULL ) {
		TrailSegment* temp = cur->getNext();
		delete cur;
		cur = temp;
	}
}

TrailSegment* Trail::getEnd() { return end; }
TrailSegment* Trail::getStart() { return start; }


void Trail::update( Vector *newBottom, Vector *newTop, Vector newColor ) {
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

			addSegment( curBottom, curTop, newColor );

			toAdd -= MAX_SEGMENT_LENGTH;
		}
		addSegment( newBottom, newTop, newColor );
	}

	if( limit && length > MAX_TRAIL_SEGMENTS ) {
		while( length > MAX_TRAIL_SEGMENTS ) {
			removeSegment();
			diff -= MAX_SEGMENT_LENGTH;
		}
		end->scaleToLengthB( MAX_SEGMENT_LENGTH - diff );
	}
}


void Trail::addSegment( Vector *newFB, Vector *newFT, Vector newColor ) {
	Vector *newBB = new Vector(); newBB->set(start->getFB());
	Vector *newBT = new Vector(); newBT->set(start->getFT());
	TrailSegment *newSegment = new TrailSegment( newBB, newBT, newFB, newFT, texture, (color.getX()==-1?newColor:color) );
	if( start->getNext() != NULL )
		delete start->getNext();
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

void Trail::stage( Vector trailEnd, Vector *tilt, TransparentRenderer *tr, Vector *cameraPos, Vector newColor ) {
	Vector* newBB = new Vector();
	newBB->set( start->getFB() );
	Vector* newBT = new Vector();
	newBT->set( start->getFT() );
	Vector *newFB = new Vector();
	*newFB = trailEnd;
	Vector *newFT = new Vector();
	*newFT = Add0( trailEnd, Scale0( *tilt, TRAIL_HEIGHT ) );

	TrailSegment *trailStart = new TrailSegment( newBB, newBT, newFB, newFT, texture, (color.getX()==-1?newColor:color) );
	start->setNext( trailStart );

	if( end != NULL )
		end->stage( tr, cameraPos ); // Will loop from end to front displaying hitboxes
}
