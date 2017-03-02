template<typename TYPE > 
	struct hedon_setter{
		static v8::Local<v8::Value> set(v8::Isolate * isolate ,TYPE data){ 
			return v8::Null(isolate);
		}
	};

template<typename TYPE, int N> 
	struct hedon_setter<TYPE [N]>{
		static v8::Local<v8::Value> set(v8::Isolate * isolate ,TYPE  data[N]){
			return 	hedon_setter<TYPE *>::set(isolate , data , N);
		} 
	};

template<typename TYPE> 
	struct hedon_setter<TYPE *>{ 
		static v8::Local<v8::Value> set(v8::Isolate * isolate ,TYPE  * data){ 
			int64_t i = (int64_t)data;
			return v8::Number::New(isolate,i); 
		}; 
		static v8::Local<v8::Value> set(v8::Isolate * isolate ,TYPE  * data ,int N){ 
			v8::Local<v8::Array> array = v8::Array::New(isolate,N);
			for(int i = 0 ; i < N ; i++){
				array->Set(i ,hedon_setter<TYPE>::set(isolate,data[i]) );
			};
			return array; 
		};  
	};


#define SETTER(TYPE,FUNCTION) \
template<> \
	struct hedon_setter<TYPE>{ \
		static bool mflag; \
		static v8::Local<v8::Value> set(v8::Isolate * isolate ,TYPE data){ \
			return FUNCTION(isolate,data); \
		}; \
	}; \


SETTER(char * ,v8::String::NewFromUtf8);
SETTER(bool,v8::Boolean::New);
SETTER(float,v8::Number::New);
SETTER(double,v8::Number::New);
SETTER(int32_t,v8::Number::New);
SETTER(uint32_t,v8::Number::New);
SETTER(int64_t,v8::Number::New);