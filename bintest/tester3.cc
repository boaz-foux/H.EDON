
#include <node.h>
#include "../src/hedon.h"

void(*global_pfunc)() = NULL;

int  recallback(){
	if(!global_pfunc){ 
		return 0;
	}
	global_pfunc();
	global_pfunc();
	global_pfunc();
	return 0;
}

void  callback( void(*pfunc)()){
	global_pfunc = pfunc;
	pfunc();
} 

int  callback0( int(*pfunc)()){
	return pfunc();
}


float  callback1( int i , float(*pfunc)(int)){
	return pfunc(i);
}

double  callback2(  double(*pfunc)(int,double) ,int i , double d ){
	return pfunc(i,d);
}

float  callback3( float m , float(*pfunc)(int,double) ,int i , double d ){
	return m * pfunc(i,d) ;
}


char * callbackChar( int index ,char*(*pfunc)(int)){
	return pfunc(index);
} 

int superCallback (int(*psfunc)(int(*)()) , int(*pfunc)() ){
	return psfunc(pfunc);
}

void Init( v8::Handle<v8::Object> exports) {
		NODE_SET_METHOD(exports,"callback",HEDON::BIND(callback));
		NODE_SET_METHOD(exports,"recallback",HEDON::BIND(recallback));
		NODE_SET_METHOD(exports,"callback0",HEDON::BIND(callback0));
		NODE_SET_METHOD(exports,"callback1",HEDON::BIND(callback1));
		NODE_SET_METHOD(exports,"callback2",HEDON::BIND(callback2));
		NODE_SET_METHOD(exports,"callback3",HEDON::BIND(callback3));
		NODE_SET_METHOD(exports,"callbackChar",HEDON::BIND(callbackChar));
		NODE_SET_METHOD(exports,"super",HEDON::BIND(superCallback));
}


NODE_MODULE(hello, Init);


