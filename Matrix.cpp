#include "Matrix.h"

Matrix::Matrix( double a00, double a01, double a02, double a10, double a11, double a12, double a20, double a21, double a22 ) {
	vals[0][0] = a00; vals[0][1] = a01; vals[0][2] = a02;
	vals[1][0] = a10; vals[1][1] = a11; vals[1][2] = a12;
	vals[2][0] = a20; vals[2][1] = a21; vals[2][2] = a22;
}

double Matrix::get( int i, int j ) {
	return vals[i][j];
}

void Matrix::set( int i, int j, int n ) {
	vals[i][j] = n;
}

Matrix Matrix::inverse() {
	double det = vals[0][0] * (vals[1][1] * vals[2][2] - vals[2][1] * vals[1][2]) -
	             vals[0][1] * (vals[1][0] * vals[2][2] - vals[1][2] * vals[2][0]) +
	             vals[0][2] * (vals[1][0] * vals[2][1] - vals[1][1] * vals[2][0]);
	double invdet = 1 / det;

	double i00 = (vals[1][1] * vals[2][2] - vals[2][1] * vals[1][2]) * invdet;
	double i01 = (vals[0][2] * vals[2][1] - vals[0][1] * vals[2][2]) * invdet;
	double i02 = (vals[0][1] * vals[1][2] - vals[0][2] * vals[1][1]) * invdet;
	double i10 = (vals[1][2] * vals[2][0] - vals[1][0] * vals[2][2]) * invdet;
	double i11 = (vals[0][0] * vals[2][2] - vals[0][2] * vals[2][0]) * invdet;
	double i12 = (vals[1][0] * vals[0][2] - vals[0][0] * vals[1][2]) * invdet;
	double i20 = (vals[1][0] * vals[2][1] - vals[2][0] * vals[1][1]) * invdet;
	double i21 = (vals[2][0] * vals[0][1] - vals[0][0] * vals[2][1]) * invdet;
	double i22 = (vals[0][0] * vals[1][1] - vals[1][0] * vals[0][1]) * invdet;

	return Matrix( i00, i01, i02, i10, i11, i12, i20, i21, i22 );
}

Vector Matrix::mult( Vector vec ) {
	double v1 = vals[0][0] * vec.getX() + vals[0][1] * vec.getY() + vals[0][2] * vec.getZ();
	double v2 = vals[1][0] * vec.getX() + vals[1][1] * vec.getY() + vals[1][2] * vec.getZ();
	double v3 = vals[2][0] * vec.getX() + vals[2][1] * vec.getY() + vals[2][2] * vec.getZ();

	return Vector(v1, v2, v3);
}
