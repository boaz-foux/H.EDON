#define ISOLATE v8::Isolate::GetCurrent()
#define V8ARGS v8::FunctionCallbackInfo<v8::Value>
typedef 
	void (* BINDING_FN_TYPE)(const v8::FunctionCallbackInfo<v8::Value> &);

template< typename TYPE , TYPE type >struct cache{ };

template<typename FNTYPE,FNTYPE * fn , int ...Is>
	struct callback_cache {
		static v8::Persistent<v8::Function> callback;
	};

template<typename FNTYPE,FNTYPE * fn , int...Is>
	 v8::Persistent<v8::Function>
	 callback_cache<FNTYPE,fn,Is...>::callback;



template<typename TYPE> 
	struct link{
	 	static TYPE (*getter)(const v8::Local<v8::Value> &);
	 	static v8::Local<v8::Value> (*setter)(TYPE);
	 	static std::string (*validator)(const v8::Local<v8::Value> &);
	 };

template<typename TYPE> 
	 TYPE 
	 	(*link<TYPE>::getter)
	 		(const v8::Local<v8::Value> &) = NULL;
template<typename TYPE> 
	 v8::Local<v8::Value>
	 	(*link<TYPE>::setter)
	 		(TYPE) = NULL;
template<typename TYPE> 
	 std::string 
	 	(*link<TYPE>::validator)
	 		(const v8::Local<v8::Value> &) = NULL;



template<typename TYPE> 
	struct linker{
	 	static 
	 		TYPE get(const v8::Local<v8::Value> &){}
	 	static 
	 		v8::Local<v8::Value> set(TYPE){
	 			return v8::Undefined(ISOLATE);
	 		}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &){}

	 };


template<typename TYPE> 
	struct linker<TYPE *>{
		static 
			TYPE * get(const v8::Local<v8::Value> &i){
				int64_t m = i->NumberValue();
				TYPE * p  = (TYPE *)m;
				return p;
			};
			static 
				v8::Local<v8::Value> set(TYPE * data){
					int64_t i = (int64_t)data;
					return v8::Number::New(ISOLATE,i);
				}
	 		static
	 			std::string validate(const v8::Local<v8::Value> &){
	 			return std::string();
	 		}
	};

template<>
	 struct linker<char *>{
	 	static char * get(const v8::Local<v8::Value> &i){
	 		v8::String::Utf8Value str(i);
			return *str;
	 	}
	 	static v8::Local<v8::Value> set(char * data){
	 		return v8::String::NewFromUtf8(ISOLATE, data);
	 	}
	 	static std::string validate(const v8::Local<v8::Value> &){
	 		return std::string();
	 	}
	 };

template<>
	 struct linker<double>{
	 	static 
	 		double get(const v8::Local<v8::Value> &i){
		 		return i->NumberValue();
		 	}
	 	static 
		 	v8::Local<v8::Value> set(double i){
		 		return v8::Number::New(ISOLATE,i);
		 	}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			std::string str;
	 			if(!i->IsNumber()){
	 				str += "invalid value, should be a number.";
	 			}
	 			return str;
	 		}
	 };

template<>
	 struct linker<float>{
	 	static 
	 		float get(const v8::Local<v8::Value> &i){
		 		return i->NumberValue();
		 	}
	 	static 
		 	v8::Local<v8::Value> set(float i){
		 		return v8::Number::New(ISOLATE,i);
		 	}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			return linker<double>::validate(i);
	 		}
	 };

template<>
	 struct linker<int32_t>{
	 	static 
	 		int32_t get(const v8::Local<v8::Value> &i){
		 		return i->Int32Value();
		 	}
	 	static 
		 	v8::Local<v8::Value> set(int32_t i){
		 		return v8::Number::New(ISOLATE,i);
		 	}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			return linker<double>::validate(i);
	 		}
	 };

template<>
	 struct linker<uint32_t>{
	 	static 
	 		uint32_t get(const v8::Local<v8::Value> &i){
		 		return i->Uint32Value();
		 	}
	 	static 
		 	v8::Local<v8::Value> set(uint32_t i){
		 		return v8::Number::New(ISOLATE,i);
		 	}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			return linker<double>::validate(i);
	 		}
	 };

template<>
	 struct linker<int64_t>{
	 	static 
	 		int64_t get(const v8::Local<v8::Value> &i){
		 		return i->IntegerValue();
		 	}
	 	static 
		 	v8::Local<v8::Value> set(int64_t i){
		 		return v8::Number::New(ISOLATE,i);
		 	}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			return linker<double>::validate(i);
	 		}
	 };	 

template<>
	 struct linker<bool>{
	 	static 
	 		bool get(const v8::Local<v8::Value> &i){
		 		return i->BooleanValue();
		 	}
	 	static 
		 	v8::Local<v8::Value> set(bool i){
		 		return v8::Boolean::New(ISOLATE,i);
		 	}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			return std::string();
	 		}
	 };


template<typename TYPE> 
	 struct unvoid {
		 typedef TYPE type;
	};
template<> 
	 struct unvoid<void> {
		 typedef bool type;
	};

template<typename TYPE> 
	 using void_wrapper =  typename unvoid<TYPE>::type;


template<>
	 struct linker<void>{
	 	static 
	 		void get(const v8::Local<v8::Value> &){
		 		return;
		 	}
	 	static 
		 	v8::Local<v8::Value> set( void ){
		 		return v8::Null(ISOLATE);
		 	}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &){
	 			return std::string();
	 		}
	 };





template<typename FNTYPE , FNTYPE fn ,typename TYPE , int ...Is> 
	struct linker_t : public linker<TYPE>{};

template<typename FNTYPE , FNTYPE fn ,typename R, typename ...ARGS ,int I >
struct linker_t<FNTYPE,fn,R(*)(ARGS...),I>{
	typedef R(&rtntype)(ARGS...);
		 static 
	 		rtntype get(const v8::Local<v8::Value> & i){
	 			v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(i);
	 			callback_cache<FNTYPE,fn,I>::callback.Reset(ISOLATE,callback);
	 			return linker_t<FNTYPE,fn,R(*)(ARGS...),I>::wrapper;
	 		}
	 	static 
	 		R wrapper (ARGS...args){
	 			int argc = counter<ARGS...>::n ;
	 			v8::Handle<v8::Value> argv [] = { 
					linker<ARGS>::set(args)...  
				};
				v8::Local<v8::Function> 
					callback = v8
						::Local<v8::Function>
						::New(ISOLATE,
							callback_cache<FNTYPE,fn,I>::callback);
				return linker<R>::get(callback->Call(
											v8::Null(ISOLATE),
											argc,
											argv));
	 		}

	 	static 
	 		std::string validate(const v8::Local<v8::Value> &){
	 			return std::string();
	 		}
};

static char missing_arguments [] = "missing arguments!";
static char to_many_arguments [] = "to many arguments!";


template<typename TYPE, TYPE data> 
	 struct binder_t {};
template<typename TYPE, int N, TYPE (&array)[N]> 
	 struct binder_t<TYPE[N],array>{
	 	static 
	 		void bind (const V8ARGS & arguments) {
	 			// todo
	 		};
	};
template<typename R, typename ...ARGS, R(*fn)(ARGS...)>
	struct binder_t<R(ARGS...),fn>{
		typedef R(fntype)(ARGS...);
		static 
			void bind (const V8ARGS & arguments) {
				if(!binder_t<fntype,fn>::validate(arguments)){
					return;
				}				
				R value = 
				 binder_t<fntype,fn>
				 ::invoke(arguments,indexes<ARGS...>());
				arguments
				.GetReturnValue()
				.Set(linker<R>::set(value));
			};
		template<int ...Is> static 
			R invoke (const V8ARGS & arguments ,sequence<Is...>){
				return fn( linker_t<fntype,fn,ARGS,Is>::get(arguments[Is])...);
			};
		
		static 
			bool validate (const V8ARGS & arguments){
			int length = arguments.Length(),
			 	n = counter<ARGS...>::n ;
			std::string str;
			v8::Isolate * isolate;

			if(n > length){
				str = missing_arguments;
			}
			else if(n  < length ){
				str = to_many_arguments;
			}
			if(str.empty()){
				return true;
			}
			str += "Error:" + str;
			isolate =  arguments.GetIsolate();
			isolate->ThrowException(
				v8::String::NewFromUtf8(isolate,str.data())
				);
			return false;
			};
	};






template<typename TYPE, TYPE data> struct binder :public binder_t<TYPE,data> {};

template< typename ...ARGS, void(*fn)(ARGS...)>
struct binder<void(ARGS...),fn> {
		static void (*bind) (const V8ARGS & arguments) ;
		static unvoid<void>::type rfn (ARGS... args){
			fn(args...);
			return true;
		}
};
template< typename ...ARGS, void(*fn)(ARGS...)>
void (*binder<void(ARGS...),fn>::bind) (const V8ARGS & arguments) 
=  binder_t<bool(ARGS...),binder<void(ARGS...),fn>
			::rfn>::bind;



