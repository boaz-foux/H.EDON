#include <node.h>

#include "../src/hedon.h"

int static_array[8] = { 3,4,6,7,8,99,10,676};


// template< typename TYPE ,typename ...ARGS >
// 	 void b(TYPE (*fn)(ARGS...x)){};

// template< typename TYPE ,size_t N >
// 	 void b(TYPE (*array) [N]){};

template< typename TYPE , TYPE t >struct c{ };

template< typename TYPE ,int N , TYPE (&array)[N] >
	struct c<TYPE[N],array>{
		static void b(){}
	};
template<typename R, typename ...ARGS, R(*fn)(ARGS...)>
	struct c<R(ARGS...),fn> {
		static void b(){}
	};

void foo(){}

void Init( v8::Handle<v8::Object> exports) {
	// c<int (&)[8],static_array>::b();
	c<decltype(static_array),static_array>::b();
	c<decltype(foo),foo>::b();
	// int i;

	// b(foo);
	// testing<decltype(foo),foo>::b();
	// testing< decltype(static_array), static_array>::b();
// NODE_SET_METHOD(exports,"static_array",HEDON::BIND(&static_array));
}


NODE_MODULE(hello, Init);