
#include <node.h>
#include <v8.h>
#include "../src/hedon.h"



int  callbackBinding( int(*pfunc)()){
	return pfunc();
}


int  callbackBinding3( int i , int j, int(*pfunc)(int,int)){
	return pfunc(i,j);
}


void Init( v8::Handle<v8::Object> exports) {
		NODE_SET_METHOD(exports,
		 "callbackBinding",
		 HEDON::BIND(callbackBinding3)
		 // RunCallback 
		 );
}


NODE_MODULE(hello, Init);


