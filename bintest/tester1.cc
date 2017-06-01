
#include <v8.h>

#include "../src/hedon.h"

double test0(){
	return 4.891;
}

int test1(double j ) { 
	return (int)(j) ; 
}

float test2(double i , double j) { 
	return  i*j; 
}

double test3(float i , double j, int k) { 
	return  (i*j) + k; 
}

int test10(
	int i  , double j ,float k,
	int i1  , double j1 ,float k1,
	int i2  , double j2 ,float k2,
	double m
	) { 
	return ( ((i*j) + k) + ((i1*j1) + k1) + ((i2*j2) + k2) ) +m;
}

void Init( v8::Handle<v8::Object> exports) {
   NODE_SET_METHOD(exports, "test0", HEDON::BIND(test0));
   NODE_SET_METHOD(exports, "test1", HEDON::BIND(test1));
   NODE_SET_METHOD(exports, "test2", HEDON::BIND(test2));
   NODE_SET_METHOD(exports, "test3", HEDON::BIND(test3));
   NODE_SET_METHOD(exports, "test10", HEDON::BIND(test10));
}


NODE_MODULE(hello, Init);


