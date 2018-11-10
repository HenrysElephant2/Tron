#include "Vector.h"
#include <cstdio>

Vector::Vector(double X, double Y, double Z)
{
	x = X;
	y = Y;
	z = Z;
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


void Vector::Add(Vector * v)// modifies this Vector
{
	x += v.getX();
	y += v.getY();
	z += v.getZ();
}
void Vector::Add(Vector * v, double scale) // modifies this vector by adding a scaled version of vector v
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

void Vector::Rotate(double angle, Vector * axis)
{
	double axisX = axis->getX();
	double axisY = axis->getY();
	double axisZ = axis->getZ();
	double angle_cos = cos(angle);
	double angle_sin = sin(angle);

	double new_x = (angle_cos + pow(axisX,2)*(1-angle_cos)) * x + 
		(axisX*axisY*(1-angle_cos) - axisZ*angle_sin) * y + 
		(axisX*axisZ*(1 - angle_cos) + axisY*angle_sin)*z;
	double new_y = (axisY*axisX*(1-angle_cos) + axisZ*angle_sin) * x + 
		(angle_cos + pow(axisY,2)*(1-angle_cos))*y + 
		(axisY*axisZ*(1-angle_cos) - axisX*angle_sin)*z;
	double new_z = (axisX*axisZ*(1-angle_cos) - axisY*angle_sin) * x + 
		(axisZ*axisY*(1-angle_cos) + axisX*angle_sin)*y + 
		(angle_cos + pow(axisZ,2) * (1-angle_cos)) * z;
	x = new_x;
	y = new_y;
	z = new_z;
}

