#include "Matrix.h"

Matrix::Matrix( double a00, double a01, double a02, double a10, double a11, double a12, double a20, double a21, double a22 ) {
	vals[0][0] = a00; vals[0][1] = a01; vals[0][2] = a02;
	vals[1][0] = a10; vals[1][1] = a11; vals[1][2] = a12;
	vals[2][0] = a20; vals[2][1] = a21; vals[2][2] = a22;

	// vals = {{a00, a01, a02}, {a10, a11, a12}, {a20, a21, a22}}
}

double Matrix::get( int i, int j ) {
	return vals[i][j];
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

int main() {
	Matrix testM(1,2,3,0,1,4,5,6,0);
	Matrix testI = testM.inverse();
	for( int i=0; i<3; i++ ) {
		for( int j=0; j<3; j++ ) {
			printf("%3f ", testI.get(i,j));
		}
		printf("\n");
	}
	return 0;
}