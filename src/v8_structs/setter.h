template<typename TYPE > 
	struct hedon_setter{};

template<typename TYPE> 
	struct hedon_setter<TYPE *>{ 
		static v8::Local<v8::Value> set(v8::Isolate * isolate ,TYPE  * data){ 
			return v8::Number::New(isolate,(int)data); 
		}; 
	};

#define SETTER(TYPE,FUNCTION) \
template<> \
	struct hedon_setter<TYPE>{ \
		static v8::Local<v8::Value> set(v8::Isolate * isolate ,TYPE data){ \
			return FUNCTION(isolate,data); \
		}; \
	}

SETTER(bool,v8::Boolean::New);
SETTER(float,v8::Number::New);
SETTER(double,v8::Number::New);
SETTER(int32_t,v8::Number::New);
SETTER(uint32_t,v8::Number::New);
SETTER(int64_t,v8::Number::New);