#include <node.h>

#include "../src/hedon.h"

int static_array[8] = { 3,4,6,7,8,99,10,676};


template< typename TYPE ,typename ...ARGS >
	 void b(TYPE (*fn)(ARGS...x)){};

template< typename TYPE ,size_t N >
	 void b(TYPE (*array) [N]){};

void foo(){}

void Init( v8::Handle<v8::Object> exports) {
//	b(&static_array);
	// b(foo);
	// testing<decltype(foo),foo>::b();
	// testing< decltype(static_array), static_array>::b();
// NODE_SET_METHOD(exports,"static_array",HEDON::BIND(&static_array));
}


NODE_MODULE(hello, Init);