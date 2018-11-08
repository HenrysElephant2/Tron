#include "Vector.h"


Vector::Vector(double X, double Y, double Z)
{
	x = X;
	y = Y;
	z = Z;
}

double getMagnitude()
{
	return sqrt(x*x + y*y + z*z);
}
// accessors
double Vector::getX(){
	return x;
}

double Vector::getY(){
	return y;
}
double Vector::getZ(){
	return z;
}
// set functions
double Vector::setX(double n){
	x = n;
}
double Vector::setY(double n){
	y = n;
}
double Vector::setZ(double n){
	z = n
}


void Vector::Add(Vector v)// modifies this Vector
{
	x += v.getX();
	y += v.getY();
	z += v.getZ();
}
void Vector::Add(Vector v, double scale) // modifies this vector by adding a scaled version of vector v
{
	x += v.getX() * scale;
	y += v.getY() * scale;
	z += v.getZ() * scale;
}

void Scale(double scale) // multiplies x y and z of this vector by scale
{
	x *= scale;
	y *= scale;
	z *= scale;
}