
#include <node.h>

#include "../src/hedon.h"

struct test1{
	int i;
	std::string str;
};

test1 getStruct (){
	test1 t;
	t.i = 777;
	t.str = "lorem ipsum dolor sit amet ac in risus ligula nisl rutrum odio et mattis et lorem ut morbi";
	return t;
}

bool tester (const test1 t1,const test1 t2){
	return t1.i == t2.i && t1.str == t2.str;
}



void Init( v8::Handle<v8::Object> exports) {
	BIND_STRUCT(test1
			,BIND_STRUCT_KEY(test1,i) ,BIND_STRUCT_KEY(test1,str)
		);
	NODE_SET_METHOD(exports,"tester",HEDON::BIND(tester));
	NODE_SET_METHOD(exports,"get",HEDON::BIND(getStruct));
}


NODE_MODULE(hello, Init);