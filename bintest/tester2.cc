
#include <node.h>
#include <v8.h>
#include "../src/hedon.h"

int i = 502;

int * getPointer(){
	return &i;
}

int  getValue(){
	return i;
}

int64_t  getValueFromPointer(int *f){
	return *f;
}

char * getString(){
	return "this is a string";
}


void Init( v8::Handle<v8::Object> exports) {
	NODE_SET_METHOD(exports, "getString", HEDON::BIND(getString));
	NODE_SET_METHOD(exports, "getValue", HEDON::BIND(getValue));
	NODE_SET_METHOD(exports, "getPointer", HEDON::BIND(getPointer));
	NODE_SET_METHOD(exports, "getValueFromPointer", HEDON::BIND(getValueFromPointer));
}


NODE_MODULE(hello, Init);


