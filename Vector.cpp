#include "Vector.h"

Vector::Vector(){}

Vector::Vector(double X, double Y, double Z)
{
	x = X;
	y = Y;
	z = Z;
}
Vector::Vector(Vector v1, Vector v2)
{
	newVec = Add0( v2, Scale0(v1, -1) ); // v2 - v1
	x = newVec.getX();
	y = newVec.getY();
	z = newVec.getZ();
}

double Vector::getMagnitude()
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
void Vector::setX(double n){
	x = n;
}
void Vector::setY(double n){
	y = n;
}
void Vector::setZ(double n){
	z = n;
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

void Vector::Scale(double scale) // multiplies x y and z of this vector by scale
{
	x *= scale;
	y *= scale;
	z *= scale;
}

// Dot product with v
double Vector::Dot( Vector v ) {
	return x * v.getX() + y * v.getY() + z * v.getZ();
}

// Cross product with v (this x v)
Vector Vector::Cross( Vector v ) {
	double vi = y * v.getZ() - z * v.getY();
	double vj = z * v.getX() - x * v.getZ();
	double vk = x * v.getY() - y * v.getX();
	return Vector(vi, vj, vk);
}

// Angle between this and v (right hand from this to v)
double Vector::Angle( Vector v ) {
	return acos( this->Dot(v) / (this->getMagnitude() * v.getMagnitude()) );
}







Vector Add0( Vector v1, Vector v2 ) {
	return Vector(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

Vector Scale0( Vector v, double scale ) {
	return Vector(v.x*scale, v.y*scale, v.z*scale);
}
