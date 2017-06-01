
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

v8::Local<v8::Value> setter (const test1 t,v8::Isolate *i){
	v8::Local<v8::Object> res = v8::Object::New(i);
	res->Set( v8::String::NewFromUtf8(i,"i"),v8::Number::New(i,t.i) );
	res->Set( v8::String::NewFromUtf8(i,"str"),v8::String::NewFromUtf8(i,t.str.data() ) );
	return res;
}

test1 getter (const v8::Local<v8::Value> &i,v8::Isolate *isolate){
	v8::Local<v8::Object> o =  i->ToObject(isolate);
	test1 t;
	t.i = o->Get(v8::String::NewFromUtf8(isolate,"i"))-> Int32Value();
	v8::String::Utf8Value str(o->Get(v8::String::NewFromUtf8(isolate,"str")));
	t.str = *str;
	return t;
}

std::string validate (const v8::Local<v8::Value> &in,v8::Isolate *isolate){
	if(!in->IsObject()){
	 return "is not an object!";	
	}
	std::string rtn;
	v8::Local<v8::Object> o =  in->ToObject(isolate);
	v8::Local<v8::String> i = v8::String::NewFromUtf8(isolate,"i"),
				str = v8::String::NewFromUtf8(isolate,"str");
	if(o->Get(i)->IsUndefined() || o->Get(i)->IsNull()){
		rtn = "i : is not defined";
	}
	else if(!o->Get(i)-> IsNumber()){
		rtn = "i : is not a number";
	}
	if(o->Get(str)->IsUndefined()){
		rtn = rtn.empty() ? rtn : rtn + ", "; 
		 rtn += "str : is not defined";
	}else if(!o->Get(str)->IsString()){
		rtn = rtn.empty() ? rtn : rtn + ", ";
		rtn += "str : is not a string";
	}
	if(!rtn.empty()){
		rtn  = "object error {" +  rtn + " }";
	}  
	return rtn;
}


bool tester (const test1 t1,const test1 t2){
	return t1.i == t2.i && t1.str == t2.str;
}

void Init( v8::Handle<v8::Object> exports) {
	HEDON::TAG(test1,"Tester1");
	HEDON::SETTER(test1, setter);
	HEDON::GETTER(test1, getter);
	HEDON::VALIDATOR(test1,validate);
	NODE_SET_METHOD(exports,"tester",HEDON::BIND(tester));
	NODE_SET_METHOD(exports,"get",HEDON::BIND(getStruct));
}


NODE_MODULE(hello, Init);