#include <node.h>

#include "../src/hedon.h"

int static_array[8] = { 3,4,6,7,8,99,10,676};
static const int i = 5558;
void Init( v8::Handle<v8::Object> exports) {
	// HEDON::Object( ("hello",1) ); 

	NODE_SET_METHOD(exports,"staticArray",HEDON::BIND(static_array));

	NODE_SET_METHOD(exports,"i",HEDON::BIND(i));

}


NODE_MODULE(hello, Init);