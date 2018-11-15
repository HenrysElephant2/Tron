#include <iostream>

using namespace std;

int* add( int* a, int* b ) {
	int ret = *a + *b;
	return &ret;
}

int main() {
	int *t1 = new int(10);
	int *t2 = new int(20);

	int *t3 = add( t1, add( t1, t2 ) );

	cout << *t1 << " + " << *t1 << " + " << *t2 << " = " << *t3 << endl;

	delete t1;
	delete t2;
	delete t3;

	return 0;
}