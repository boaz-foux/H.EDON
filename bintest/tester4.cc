
#include <node.h>

#include "../src/hedon.h"

int staticArray[8] = { 3,4,6,7,8,99,10,676};

static const int i = 5558;

void Init( v8::Handle<v8::Object> exports) {
	HEDON::EXPORT(exports,staticArray);
	HEDON::EXPORT(exports,i);
}


NODE_MODULE(hello, Init);