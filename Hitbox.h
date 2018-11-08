#ifndef HITBOX_H
#define HITBOX_H

class HPoint{
public:
	double x, y, z;

	HPoint( double ix, double iy, double iz );
	void move( double dx, double dy, double dz );
};

class HVec{
public:
	HPoint point;
	HPoint vec;

	HVec( HPoint p, HPoint v );
};


class Hitbox {
//private:
};

#endif