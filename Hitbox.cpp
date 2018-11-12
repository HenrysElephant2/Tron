#include "Hitbox.h"

Hitbox::Hitbox( Vector p, Vector f, Vector u, double w, double l, double h ) {
	pos = p;
	forward = Scale0( f, 1/f.getMagnitude() ); // Normalize
	up = Scale0( u, 1/u.getMagnitude() ); // Normalize
	cross = forward.Cross(up); // Calculate cross product
	cross.Scale( 1/cross.getMagnitude() ); // Normalize cross product
	width = w;
	length = l;
	height = h;

	calculatePoints();
}

// Accessors
Vector Hitbox::getPos() { return pos; }
Vector Hitbox::getForward() { return forward; }
Vector Hitbox::getUp() { return up; }
double Hitbox::getWidth() { return width; }
double Hitbox::getLength() { return length; }
double Hitbox::getHeight() { return height; }
Vector Hitbox::getPoint( int i ) { return points[i]; }

// Modifiers
void Hitbox::setPos( Vector toSet ) { pos = toSet; }
void Hitbox::setForward( Vector toSet ) { forward = toSet; }
void Hitbox::setUp( Vector toSet ) { up = toSet; }
void Hitbox::setWidth( double toSet ) { width = toSet; }
void Hitbox::setLength( double toSet ) { length = toSet; }
void Hitbox::setHeight( double toSet ) { height = toSet; }

void Hitbox::move( Vector dv ) {
	pos.Add(dv);
	calculatePoints();
}

void Hitbox::roll( double degrees ) {
	up.Rotate(degrees, &forward);
	cross.Rotate(degrees, &forward);
	calculatePoints();
}

void Hitbox::pitch( double degrees ) {
	up.Rotate(degrees, &cross);
	forward.Rotate(degrees, &cross);
	calculatePoints();
}

void Hitbox::yaw( double degrees ) {
	forward.Rotate(degrees, &up);
	cross.Rotate(degrees, &up);
	calculatePoints();
}

void Hitbox::renderSelf( bool colliding ) {
	glPushMatrix();
	colliding ? glColor3d(1,0,0) : glColor3d(0,1,0); // Red if colliding, green if not
	// glColor3d(1,0,0);
	// Back face
	glBegin(GL_LINE_LOOP);
		points[BBL].gl();
		points[BBR].gl();
		points[TBR].gl();
		points[TBL].gl();
	glEnd();
	// Front face
	glBegin(GL_LINE_LOOP);
		points[BFL].gl();
		points[BFR].gl();
		points[TFR].gl();
		points[TFL].gl();
	glEnd();
	// Connecting lines
	glBegin(GL_LINES);
		points[BBL].gl(); points[BFL].gl();
		points[BBR].gl(); points[BFR].gl();
		points[TBR].gl(); points[TFR].gl();
		points[TBL].gl(); points[TFL].gl();
	glEnd();
	glPopMatrix();
}

bool testCollision( Hitbox h1, Hitbox h2 ) {
	return testCollisionHelper(h1, h2) || testCollisionHelper(h2, h1);
}

bool testCollisionHelper( Hitbox h1, Hitbox h2 ) {
	// l, m, and n form a new coordinate system to convert h2's coordinates into and test using AABB.
	Vector l( h1.points[BBL], h1.points[BBR] );
	Vector m( h1.points[BBL], h1.points[BFL] );
	Vector n( h1.points[BBL], h1.points[TBL] );

	Matrix basis(l.getX(), m.getX(), n.getX(),
				 l.getY(), m.getY(), n.getY(),
				 l.getZ(), m.getZ(), n.getZ());

	Matrix change = basis.inverse();

	Vector tPoints[8]; // Translated relative to bbl of h1
	for( int i=0; i<8; i++ ) {
		tPoints[i] = change.mult( Add0( h2.points[i], Scale0( h1.points[BBL], -1 ) ) );
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

	for( int i=0; i<12; i++ ) {
		double t;
		Vector newVec;

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
	points[BBL] = Add0( Add0( pos, Scale0(forward, -length/2) ),  Scale0(cross, -width/2) );
	points[BBR] = Add0( Add0( pos, Scale0(forward, -length/2) ),  Scale0(cross, +width/2) );
	points[BFR] = Add0( Add0( pos, Scale0(forward, +length/2) ),  Scale0(cross, +width/2) );
	points[BFL] = Add0( Add0( pos, Scale0(forward, +length/2) ),  Scale0(cross, -width/2) );
	points[TBL] = Add0( Add0( Add0( pos, Scale0(forward, -length/2) ),  Scale0(cross, -width/2) ), Scale0(up, height) );
	points[TBR] = Add0( Add0( Add0( pos, Scale0(forward, -length/2) ),  Scale0(cross, +width/2) ), Scale0(up, height) );
	points[TFR] = Add0( Add0( Add0( pos, Scale0(forward, +length/2) ),  Scale0(cross, +width/2) ), Scale0(up, height) );
	points[TFL] = Add0( Add0( Add0( pos, Scale0(forward, +length/2) ),  Scale0(cross, -width/2) ), Scale0(up, height) );
}
