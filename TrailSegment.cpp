#include "TrailSegment.h"

TrailSegment::TrailSegment(){}

TrailSegment::TrailSegment( Vector *_bb, Vector *_bt, Vector *_fb, Vector *_ft ) {
	bb = _bb;
	bt = _bt;
	fb = _fb;
	ft = _ft;
	next = NULL;
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

void TrailSegment::updateFront( Vector *bPos, Vector *tPos ) {
	fb = bPos;
	ft = tPos;
	calculateHitbox();
}

void TrailSegment::updateBack( Vector *bPos, Vector *tPos ) {
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