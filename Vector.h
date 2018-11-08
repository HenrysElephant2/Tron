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
	double setX();
	double setY();
	double setZ();


	void Add(Vector v);// modifies this Vector
	void Add(Vector v, double scale); // modifies this vector by adding a scaled version of vector v

	void Scale(double scale); // multiplies x y and z of this vector by scale




};