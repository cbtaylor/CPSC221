//============================================================================
// Name        : helloworld.cpp
// Author      : bt
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	int sum = 0;

	for (int i = 1; i<11; i++) {
		sum += i;
		cout << "i: " << i << " sum: " << sum << endl;

	}

	int A[10];
	for (int i=0; i<10; i++) {
		A[i] = i*i + 1;
	}

	for (int i = 0; i<10; i++) {
		cout << "A[" << i << "] = " << A[i] << endl;
	}

	enum Color {red, blue, yellow, orange, black, white};

	Color aCol = red;
	Color bCol = black;

	int *p, x;
	x = 3;
	p = &x;
	cout << *p << endl;;
	*p = 5;
	cout << x << endl;
	cout << p;


	return 0;
}

struct Student {
	int id;
	bool isGrad;
};

int NumGrads (Student S[], int size) {
	int numGrads = 0;
	for (int i=0; i<size; i++) {
		if (S[i].isGrad) numGrads++;
	}
	return numGrads;
}

bool ArrayEq( int B[], int C[], int size ) {
	for (int i=0; i<size; i++) {
		if (B[i] != C[i]) return false;
	}
	return true;
}
