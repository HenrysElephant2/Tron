#include "TrailSegment.h"

TrailSegment::TrailSegment(){}

TrailSegment::TrailSegment( Vector *_bb, Vector *_bt, Vector *_fb, Vector *_ft, unsigned int t, Vector c ) {

	bb = _bb;
	bt = _bt;
	fb = _fb;
	ft = _ft;
	texture = t;
	color = c;
	next = NULL;
	hitbox = new Hitbox();
	calculateHitbox();
}

TrailSegment::~TrailSegment() {
	delete bb;
	delete bt;
	delete fb;
	delete ft;
	delete hitbox;
}

Vector* TrailSegment::getBB() { return bb; }
Vector* TrailSegment::getBT() { return bt; }
Vector* TrailSegment::getFB() { return fb; }
Vector* TrailSegment::getFT() { return ft; }
TrailSegment* TrailSegment::getNext() { return next; }
Hitbox* TrailSegment::getHitbox() { return hitbox; }
Vector TrailSegment::getColor() { return color; }
void TrailSegment::setNext( TrailSegment *newNext ) { next = newNext; }


Vector TrailSegment::getTopVector() {
	return Add0( *ft, Scale0( *bt, -1 ) );
}

Vector TrailSegment::getBottomVector() {
	return Add0( *fb, Scale0( *bb, -1 ) );
}

void TrailSegment::scaleToLengthF( double length, Vector tilt ) {
	Vector* newFB = new Vector();
	Vector dVec = getBottomVector();
	dVec.Scale( length / dVec.getMagnitude() );
	newFB->set( bb );
	newFB->Add( &dVec );

	Vector* newFT = new Vector();
	*newFT = Add0( *newFB, tilt );

	updateFront( newFB, newFT );
}

void TrailSegment::scaleToLengthB( double length ) {
	Vector* newBB = new Vector();
	Vector dVec = getBottomVector();
	dVec.Scale( -length / dVec.getMagnitude() );
	newBB->set( fb );
	newBB->Add( &dVec );

	Vector* newBT = new Vector();
	dVec = getTopVector();
	dVec.Scale( -length / dVec.getMagnitude() );
	newBT->set( ft );
	newBT->Add( &dVec );

	updateBack( newBB, newBT );
}

void TrailSegment::updateFront( Vector *bPos, Vector *tPos ) {
	//delete fb;
	//delete ft;
	fb = bPos;
	ft = tPos;
	calculateHitbox();
}

void TrailSegment::updateBack( Vector *bPos, Vector *tPos ) {
	// delete bb;
	// delete bt;
	bb = bPos;
	bt = tPos;
	calculateHitbox();
}

void TrailSegment::calculateHitbox() {
	Vector fVec = Add0( *fb, Scale0( *bb, -1 ) );
	Vector hPos = Add0( *bb, Scale0( fVec, .5 ) );
	Vector uVec = Add0( *ft, Scale0( *fb, -1 ) );
	hitbox->updateVecs( &hPos, &fVec, &uVec, false );
	hitbox->updateDimensions( TRAIL_WIDTH, fVec.getMagnitude(), TRAIL_HEIGHT );
}

bool TrailSegment::testSegmentHit( Hitbox *other ) {
	return testCollision(hitbox, other) || (next == NULL ? false : next->testSegmentHit(other) );
}

void TrailSegment::stage( TransparentRenderer *tr, Vector *cameraPos ) {
	double dist = Add0( *cameraPos, Scale0( *bb, -1 ) ).getMagnitude();
	// double dist2 = Add0( *cameraPos, Scale0( *fb, -1 ) ).getMagnitude();
	// double dist = dist1<dist2 ? dist1 : dist2;
	tr->add( this, dist );
	if( next != NULL ) {
		next->stage( tr, cameraPos );
	}
}

void TrailSegment::display( Vector *cameraPos ) {
	if( next != NULL ) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,texture);

		Vector lVec = Add0( *(hitbox->getPoint(BBL)), Scale0(*(hitbox->getPoint(BBR)), -1) ); lVec.Normalize();
		Vector tVec = Add0( *(hitbox->getPoint(TBL)), Scale0(*(hitbox->getPoint(BBL)), -1) ); tVec.Normalize();
		Vector rVec = Add0( *(hitbox->getPoint(TBR)), Scale0(*(hitbox->getPoint(TBL)), -1) ); rVec.Normalize();
		Vector bVec = Add0( *(hitbox->getPoint(BBR)), Scale0(*(hitbox->getPoint(TBR)), -1) ); bVec.Normalize();
		Vector testVec = Add0( Add0( *bb, Scale0( Add0( *bt, Scale0( *bb, -1 ) ), .5 ) ), Scale0( *cameraPos, -1 ) ); testVec.Normalize();

		int faces[4] = {FACE_L, FACE_T, FACE_R, FACE_B};
		double dots[4] = {testVec.Dot(&lVec), testVec.Dot(&tVec), testVec.Dot(&rVec), testVec.Dot(&bVec)};
		
		for( int i=0; i<4; i++ ) {
			for( int j=i+1; j<4; j++ ){
				if( dots[j] > dots[i] ) {
					double tempd = dots[j];
					int tempf = faces[j];
					dots[j] = dots[i];
					faces[j] = faces[i];
					dots[i] = tempd;
					faces[i] = tempf;
				}
			}
		}

		for( int i=0; i<4; i++ ) 
			displayFace( faces[i] );

		glDisable(GL_TEXTURE_2D);
	}
}

void TrailSegment::displayFace( int face ) {
	Hitbox *nhb = next->getHitbox();

	glBegin(GL_QUADS);
	switch( face ) {
		case FACE_L:
			glColor4f(color.getX(), color.getY(), color.getZ(), .5); glTexCoord2i(0,0); hitbox->getPoint(BFL)->gl();
			glColor4f(next->getColor().getX(), color.getY(), color.getZ(), .5); glTexCoord2i(1,0); nhb->getPoint(BFL)->gl();
			glColor4f(next->getColor().getX(), color.getY(), color.getZ(), .5); glTexCoord2i(1,1); nhb->getPoint(TFL)->gl();
			glColor4f(color.getX(), color.getY(), color.getZ(), .5); glTexCoord2i(0,1); hitbox->getPoint(TFL)->gl();
		break;

		case FACE_T:
			glColor4f(color.getX(), color.getY(), color.getZ(), .5); glTexCoord2i(0,1); hitbox->getPoint(TFL)->gl();
			glColor4f(next->getColor().getX(), color.getY(), color.getZ(), .5); glTexCoord2i(1,1); nhb->getPoint(TFL)->gl();
			glColor4f(next->getColor().getX(), color.getY(), color.getZ(), .5); glTexCoord2i(1,1); nhb->getPoint(TFR)->gl();
			glColor4f(color.getX(), color.getY(), color.getZ(), .5); glTexCoord2i(0,1); hitbox->getPoint(TFR)->gl();
		break;

		case FACE_R:
			glColor4f(color.getX(), color.getY(), color.getZ(), .5); glTexCoord2i(0,1); hitbox->getPoint(TFR)->gl();
			glColor4f(next->getColor().getX(), color.getY(), color.getZ(), .5); glTexCoord2i(1,1); nhb->getPoint(TFR)->gl();
			glColor4f(next->getColor().getX(), color.getY(), color.getZ(), .5); glTexCoord2i(1,0); nhb->getPoint(BFR)->gl();
			glColor4f(color.getX(), color.getY(), color.getZ(), .5); glTexCoord2i(0,0); hitbox->getPoint(BFR)->gl();
		break;

		case FACE_B:
			glColor4f(color.getX(), color.getY(), color.getZ(), .5); glTexCoord2i(0,0); hitbox->getPoint(BFR)->gl();
			glColor4f(next->getColor().getX(), color.getY(), color.getZ(), .5); glTexCoord2i(1,0); nhb->getPoint(BFR)->gl();
			glColor4f(next->getColor().getX(), color.getY(), color.getZ(), .5); glTexCoord2i(1,0); nhb->getPoint(BFL)->gl();
			glColor4f(color.getX(), color.getY(), color.getZ(), .5); glTexCoord2i(0,0); hitbox->getPoint(BFL)->gl();
		break;
	}
	glEnd();
}