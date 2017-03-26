#include <node.h>

#include "../src/hedon.h"

int static_array[8] = { 3,4,6,7,8,99,10,676};



void Init( v8::Handle<v8::Object> exports) {
// NODE_SET_METHOD(exports,"static_array",HEDON::BIND(&static_array));
}


NODE_MODULE(hello, Init);