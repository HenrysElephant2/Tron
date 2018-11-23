
#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Vector{
private:

	double x, y, z;

public:

	Vector(); // Default constructor
	Vector(double X, double Y, double Z);
	Vector(Vector v1, Vector v2); // Initialize as V2 - V1
	double getMagnitude();
	// accessors
	double getX();
	double getY();
	double getZ();
	// set functions
	void setX(double n);
	void setY(double n);
	void setZ(double n);

	void gl(); // Get vector as glVertex3d

	void Add(Vector *v);// modifies this Vector
	void Add(Vector *v, double scale); // modifies this vector by adding a scaled version of vector v
	void Scale(double scale); // multiplies x y and z of this vector by scale

	void Rotate(double angle, Vector * axis); // angle in radians. must be unit vector


	double Dot( Vector * v ); // Dot product with another vector
	Vector Cross( Vector * v ); // Cross product with another vector (this x v)
	double Angle( Vector * v ); // Angle with another vector


	// Versions that don't affect original vector, but return a new one
	friend Vector Add0( Vector v1, Vector v2 );
	friend Vector Scale0( Vector v, double scale );
};

#endif