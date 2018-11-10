
#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

class Vector{
private:

	double x, y, z;

public:

	Vector(double X, double Y, double Z);
	double getMagnitude();
	// accessors
	double getX();
	double getY();
	double getZ();
	// set functions
	void setX(double n);
	void setY(double n);
	void setZ(double n);


	void Add(Vector *v);// modifies this Vector
	void Add(Vector *v, double scale); // modifies this vector by adding a scaled version of vector v

	void Scale(double scale); // multiplies x y and z of this vector by scale

	void Rotate(double angle, Vector * axis); // angle in radians. must be unit vector




};

#endif