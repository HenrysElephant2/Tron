#include "Hitbox.h"

Hitbox::Hitbox( Vector p, Vector f, Vector u, double w, double l, double h ) {
	pos = p;
	forward = Scale0( f, 1/f.getMagnitude() ); // Normalize
	up = Scale0( u, 1/up.getMagnitude() ); // Normalize
	width = w;
	length = l;
	height = h;

	Vector cross = forward.Cross(up); // Calculate cross product
	cross.Scale( 1/cross.getMagnitude() ); // Normalize cross product

	points[BBL] = Add0( Add0( pos, Scale0(forward, -length/2) ),  Scale0(cross, -width/2) );
	points[BBR] = Add0( Add0( pos, Scale0(forward, -length/2) ),  Scale0(cross, +width/2) );
	points[BFR] = Add0( Add0( pos, Scale0(forward, +length/2) ),  Scale0(cross, +width/2) );
	points[BFL] = Add0( Add0( pos, Scale0(forward, +length/2) ),  Scale0(cross, -width/2) );
	points[TBL] = Add0( Add0( Add0( pos, Scale0(forward, -length/2) ),  Scale0(cross, -width/2) ), Scale0(up, height) );
	points[TBR] = Add0( Add0( Add0( pos, Scale0(forward, -length/2) ),  Scale0(cross, +width/2) ), Scale0(up, height) );
	points[TFR] = Add0( Add0( Add0( pos, Scale0(forward, +length/2) ),  Scale0(cross, +width/2) ), Scale0(up, height) );
	points[TFL] = Add0( Add0( Add0( pos, Scale0(forward, +length/2) ),  Scale0(cross, -width/2) ), Scale0(up, height) );
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

bool testCollision( Hitbox h1, Hitbox h2 ) {
	// l, m, and n form a new coordinate system to convert h2's coordinates into and test using AABB.
	Vector l( h1.points[BBL], h1.points[BBR] );
	Vector m( h1.points[BBL], h1.points[BFL] );
	Vector n( h1.points[BBL], h1.points[TBL] );

	Matrix basis(l.getX(), m.getX(), n.getX(),
				 l.getY(), m.getY(), n.getY(),
				 l.getZ(), m.getZ(), n.getZ());

	Matrix change = basis.inverse();

	for( int i=0; i<8; i++ ) {
		Vector newCoords = change.mult( Add0(h2.points[i], Scale0(h1.points[BBL], -1)) );
		if( newCoords.getX() >= 0 && newCoords.getX() <= 1 &&
			newCoords.getY() >= 0 && newCoords.getY() <= 1 &&
			newCoords.getZ() >= 0 && newCoords.getZ() <= 1 ) {
			return true;
		}
	}
	return false;
}

int main() {
	return 0;
}
