#include "Hitbox.h"
 
using namespace std;

HVec::HVec( HPoint p, HPoint v ) {
	point = p;
	vec = v;
}

double HVec::angle( HVec other ) {
	// TODO
	return 0;
}


HPoint::HPoint( double ix, double iy, double iz ) {
	x = ix;
	y = iy;
	z = iz;
}

void HPoint::move( double dx, double dy, double dz ) {
	x += dx;
	y += dy;
	z += dz;
}