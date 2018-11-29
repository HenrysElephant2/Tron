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
	TrailSegment *newSegment = new TrailSegment( newBB, newBT, newFB, newFT, texture, color );
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

void Trail::stage( Vector trailEnd, Vector *tilt, TransparentRenderer *tr, Vector *cameraPos ) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);
	if( end != NULL ) {
		end->stage( tr, cameraPos ); // Will loop from end to front displaying hitboxes
	}

	Vector renderF = Add0( trailEnd, Scale0( *(start->getFB()), -1 ) );
	Vector renderPos = Add0( *(start->getFB()), Scale0( renderF, .5 ) );
	Hitbox renderBox( &renderPos, &renderF, tilt, TRAIL_WIDTH, renderF.getMagnitude(), TRAIL_HEIGHT );

	Vector curColor = start->getColor();
	glColor4f( curColor.getX(), curColor.getY(), curColor.getZ(), .5 );

	glDepthMask(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUAD_STRIP);
		glTexCoord2i(1,0); start->getHitbox()->getPoint(BFL)->gl();
		glTexCoord2i(0,0); renderBox.getPoint(BFL)->gl();
		glTexCoord2i(1,1); start->getHitbox()->getPoint(TFL)->gl();
		glTexCoord2i(0,1); renderBox.getPoint(TFL)->gl();
		glTexCoord2i(1,1); start->getHitbox()->getPoint(TFR)->gl();
		glTexCoord2i(0,1); renderBox.getPoint(TFR)->gl();
		glTexCoord2i(1,0); start->getHitbox()->getPoint(BFR)->gl();
		glTexCoord2i(0,0); renderBox.getPoint(BFR)->gl();
		glTexCoord2i(1,0); start->getHitbox()->getPoint(BFL)->gl();
		glTexCoord2i(0,0); renderBox.getPoint(BFL)->gl();
	glEnd();
	glDepthMask(1);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}