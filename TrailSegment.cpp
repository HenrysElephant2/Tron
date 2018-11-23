#include "TrailSegment.h"

TrailSegment::TrailSegment(){}

TrailSegment::TrailSegment( Vector *_bb, Vector *_bt, Vector *_fb, Vector *_ft ) {

	bb = _bb;
	bt = _bt;
	fb = _fb;
	ft = _ft;
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
void TrailSegment::setNext( TrailSegment *newNext ) { next = newNext; }


Vector TrailSegment::getTopVector() {
	return Add0( *ft, Scale0( *bt, -1 ) );
}

Vector TrailSegment::getBottomVector() {
	return Add0( *fb, Scale0( *bb, -1 ) );
}

void TrailSegment::scaleToLengthF( double length ) {
	Vector* newFB = new Vector();
	Vector dVec = getBottomVector();
	dVec.Scale( length / dVec.getMagnitude() );
	newFB->set( bb );
	newFB->Add( &dVec );

	Vector* newFT = new Vector();
	dVec = getTopVector();
	dVec.Scale( length / dVec.getMagnitude() );
	newFT->set( bt );
	newFT->Add( &dVec );

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

void TrailSegment::display() {
	hitbox->renderSelf(false);
	if( next != NULL ) {
		next->display();
	}
}