#include "Hitbox.h"

Hitbox::Hitbox() {
	pos = new Vector();
	forward = new Vector();
	up = new Vector();
	cross = new Vector();
	for( int i=0; i<8; i++ ) {
		points[i] = new Vector();
	}
}

Hitbox::Hitbox( Vector *p, Vector *f, Vector *u, double w, double l, double h ) {
	pos = new Vector();
	forward = new Vector();
	up = new Vector();
	cross = new Vector();
	for( int i=0; i<8; i++ ) {
		points[i] = new Vector();
	}
	
	updateVecs( p, f, u, false );
	updateDimensions( w, l, h );
}

Hitbox::~Hitbox() {
	delete pos;
	delete forward;
	delete up;
	delete cross;
	for( int i=0; i<8; i++ ) {
		delete points[i];
	}
}

void Hitbox::updateVecs( Vector *newPos, Vector *newForward, Vector *newUp, bool calc ) {
	pos->set(newPos);
	forward->set(newForward); forward->Normalize(); // Normalize
	up->set(newUp); up->Normalize(); // Normalize
	cross->set(forward->Cross(up)); cross->Normalize(); // Normalize cross product
	if( calc )
		calculatePoints();
}
void Hitbox::updateDimensions( double newW, double newL, double newH, bool calc ) {
	width = newW;
	length = newL;
	height = newH;
	if( calc )
		calculatePoints();
}

// Accessors
Vector* Hitbox::getPos() { return pos; }
Vector* Hitbox::getForward() { return forward; }
Vector* Hitbox::getUp() { return up; }
double Hitbox::getWidth() { return width; }
double Hitbox::getLength() { return length; }
double Hitbox::getHeight() { return height; }
Vector* Hitbox::getPoint( int i ) { return points[i]; }

// Modifiers
void Hitbox::setPos( Vector *toSet ) { pos = toSet; }
void Hitbox::setForward( Vector *toSet ) { forward = toSet; }
void Hitbox::setUp( Vector *toSet ) { up = toSet; }
void Hitbox::setWidth( double toSet ) { width = toSet; }
void Hitbox::setLength( double toSet ) { length = toSet; }
void Hitbox::setHeight( double toSet ) { height = toSet; }

void Hitbox::move( Vector *dv ) {
	pos->Add(dv);
	calculatePoints();
}

void Hitbox::roll( double degrees ) {
	up->Rotate(degrees, forward);
	cross->Rotate(degrees, forward);
	calculatePoints();
}

void Hitbox::pitch( double degrees ) {
	up->Rotate(degrees, cross);
	forward->Rotate(degrees, cross);
	calculatePoints();
}

void Hitbox::yaw( double degrees ) {
	forward->Rotate(degrees, up);
	cross->Rotate(degrees, up);
	calculatePoints();
}


void Hitbox::renderSelf( bool colliding ) {
	glPushMatrix();
	// glEnable(GL_LINE_SMOOTH);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glLineWidth(3);

	colliding ? glColor3d(1,0,0) : glColor3d(0,1,0); // Red if colliding, green if not
	// Back face
	glBegin(GL_LINE_LOOP);
		points[BBL]->gl();
		points[BBR]->gl();
		points[TBR]->gl();
		points[TBL]->gl();
	glEnd();
	// Front face
	glBegin(GL_LINE_LOOP);
		points[BFL]->gl();
		points[BFR]->gl();
		points[TFR]->gl();
		points[TFL]->gl();
	glEnd();
	// Connecting lines
	glBegin(GL_LINES);
		points[BBL]->gl(); points[BFL]->gl();
		points[BBR]->gl(); points[BFR]->gl();
		points[TBR]->gl(); points[TFR]->gl();
		points[TBL]->gl(); points[TFL]->gl();
	glEnd();


	// glDisable(GL_LINE_SMOOTH);
	// glDisable(GL_BLEND);
	glPopMatrix();
}

bool testCollision( Hitbox *h1, Hitbox *h2 ) {
	// l, m, and n form a new coordinate system to convert h2's coordinates into and test using AABB.
	Vector l( h1->points[BBL], h1->points[BBR] );
	Vector m( h1->points[BBL], h1->points[BFL] );
	Vector n( h1->points[BBL], h1->points[TBL] );

	Matrix basis(l.getX(), m.getX(), n.getX(),
				 l.getY(), m.getY(), n.getY(),
				 l.getZ(), m.getZ(), n.getZ());

	Matrix change = basis.inverse();

	Vector tPoints[8]; // Translated relative to bbl of h1
	for( int i=0; i<8; i++ ) {
		tPoints[i] = change.mult( Add0( *(h2->points[i]), Scale0( *(h1->points[BBL]), -1 ) ) );
	}

	Vector bVec[12] = {
		tPoints[BBL],
		tPoints[BBR],
		tPoints[TBR],
		tPoints[TBL],
		tPoints[BFL],
		tPoints[BFR],
		tPoints[TFR],
		tPoints[TFL],
		tPoints[BBL],
		tPoints[BBR],
		tPoints[TBR],
		tPoints[TBL]
	};

	Vector mVec[12] = {
		Add0( tPoints[BBR], Scale0( tPoints[BBL], -1 ) ),
		Add0( tPoints[TBR], Scale0( tPoints[BBR], -1 ) ),
		Add0( tPoints[TBL], Scale0( tPoints[TBR], -1 ) ),
		Add0( tPoints[BBL], Scale0( tPoints[TBL], -1 ) ),
		Add0( tPoints[BFR], Scale0( tPoints[BFL], -1 ) ),
		Add0( tPoints[TFR], Scale0( tPoints[BFR], -1 ) ),
		Add0( tPoints[TFL], Scale0( tPoints[TFR], -1 ) ),
		Add0( tPoints[BFL], Scale0( tPoints[TFL], -1 ) ),
		Add0( tPoints[BFL], Scale0( tPoints[BBL], -1 ) ),
		Add0( tPoints[BFR], Scale0( tPoints[BBR], -1 ) ),
		Add0( tPoints[TFR], Scale0( tPoints[TBR], -1 ) ),
		Add0( tPoints[TFL], Scale0( tPoints[TBL], -1 ) )
	};

	double t;
	Vector newVec;
	for( int i=0; i<12; i++ ) {
		// Test intersection with plane x=0
		t = - bVec[i].getX() / mVec[i].getX();
		if( t >= 0 && t <= 1 ) {
			newVec = Add0( bVec[i], Scale0( mVec[i], t ) );
			if( newVec.getY() >= 0 && newVec.getY() <= 1 && newVec.getZ() >= 0 && newVec.getZ() <= 1 ) {
				return true;
			}
		}

		// Test intersection with plane x=1
		t = (1 - bVec[i].getX()) / mVec[i].getX();
		if( t >= 0 && t <= 1 ) {
			newVec = Add0( bVec[i], Scale0( mVec[i], t ) );
			if( newVec.getY() >= 0 && newVec.getY() <= 1 && newVec.getZ() >= 0 && newVec.getZ() <= 1 ) {
				return true;
			}
		}

		// Test intersection with plane y=0
		t = - bVec[i].getY() / mVec[i].getY();
		if( t >= 0 && t <= 1 ) {
			newVec = Add0( bVec[i], Scale0( mVec[i], t ) );
			if( newVec.getX() >= 0 && newVec.getX() <= 1 && newVec.getZ() >= 0 && newVec.getZ() <= 1 ) {
				return true;
			}
		}

		// Test intersection with plane y=1
		t = (1 - bVec[i].getY()) / mVec[i].getY();
		if( t >= 0 && t <= 1 ) {
			newVec = Add0( bVec[i], Scale0( mVec[i], t ) );
			if( newVec.getX() >= 0 && newVec.getX() <= 1 && newVec.getZ() >= 0 && newVec.getZ() <= 1 ) {
				return true;
			}
		}

		// Test intersection with plane z=0
		t = - bVec[i].getZ() / mVec[i].getZ();
		if( t >= 0 && t <= 1 ) {
			newVec = Add0( bVec[i], Scale0( mVec[i], t ) );
			if( newVec.getX() >= 0 && newVec.getX() <= 1 && newVec.getY() >= 0 && newVec.getY() <= 1 ) {
				return true;
			}
		}

		// Test intersection with plane z=1
		t = (1 - bVec[i].getZ()) / mVec[i].getZ();
		if( t >= 0 && t <= 1 ) {
			newVec = Add0( bVec[i], Scale0( mVec[i], t ) );
			if( newVec.getX() >= 0 && newVec.getX() <= 1 && newVec.getY() >= 0 && newVec.getY() <= 1 ) {
				return true;
			}
		}
	}

	return false;
}


void Hitbox::calculatePoints() {
	Vector pBBL = Add0( Add0( *pos, Scale0(*forward, -length/2) ),  Scale0(*cross, -width/2) );
	Vector pBBR = Add0( Add0( *pos, Scale0(*forward, -length/2) ),  Scale0(*cross, +width/2) );
	Vector pBFR = Add0( Add0( *pos, Scale0(*forward, +length/2) ),  Scale0(*cross, +width/2) );
	Vector pBFL = Add0( Add0( *pos, Scale0(*forward, +length/2) ),  Scale0(*cross, -width/2) );
	Vector pTBL = Add0( Add0( Add0( *pos, Scale0(*forward, -length/2) ),  Scale0(*cross, -width/2) ), Scale0(*up, height) );
	Vector pTBR = Add0( Add0( Add0( *pos, Scale0(*forward, -length/2) ),  Scale0(*cross, +width/2) ), Scale0(*up, height) );
	Vector pTFR = Add0( Add0( Add0( *pos, Scale0(*forward, +length/2) ),  Scale0(*cross, +width/2) ), Scale0(*up, height) );
	Vector pTFL = Add0( Add0( Add0( *pos, Scale0(*forward, +length/2) ),  Scale0(*cross, -width/2) ), Scale0(*up, height) );
	points[BBL]->set(&pBBL);
	points[BBR]->set(&pBBR);
	points[BFR]->set(&pBFR);
	points[BFL]->set(&pBFL);
	points[TBL]->set(&pTBL);
	points[TBR]->set(&pTBR);
	points[TFR]->set(&pTFR);
	points[TFL]->set(&pTFL);
}
