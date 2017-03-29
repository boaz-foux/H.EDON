
#include <node.h>

#include "../src/hedon.h"

typedef struct {
	int64_t data;
} unknown_struct;

unknown_struct i;

unknown_struct * getPointer(){
	return &i;
}

int  getValue(){
	return i.data;
}

int64_t  getValueFromPointer(unknown_struct *f){
	return f->data;
}

char * getString(){
	char * t = "this is a string";
	return t;
}

const char * getString2( const char * m){
	return m;
}


void Init( v8::Handle<v8::Object> exports) {
	i.data = 502;
	NODE_SET_METHOD(exports, "getString", HEDON::BIND(getString));
	NODE_SET_METHOD(exports, "getString2", HEDON::BIND(getString2));
	NODE_SET_METHOD(exports, "getValue", HEDON::BIND(getValue));
	NODE_SET_METHOD(exports, "getPointer", HEDON::BIND(getPointer));
	NODE_SET_METHOD(exports, "getValueFromPointer", HEDON::BIND(getValueFromPointer));
}


NODE_MODULE(hello, Init);


