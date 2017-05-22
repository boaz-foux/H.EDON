#ifndef ___PURE__HEDON__BIND___H___
#define ___PURE__HEDON__BIND___H___
#include <string>


namespace HEDON {

#define BIND(I) binder<decltype(I),I>::bind
#define BIND_OBJ(I,...)
				
#define GETTER(C,GET) link<C>::getter = GET
#define SETTER(C,SET) link<C>::setter = SET
#define VALIDATOR(C,VALIDATE) link<C>::validator = VALIDATE

/*
	counter
*/ 
template< typename ... ARGS> 
	struct counter {};
template<>
	struct counter<> {
	    static const int n = 0;
	};
template<typename T, typename... VARGS>
	struct counter<T, VARGS...> {
	    static const int n = 1 + counter<VARGS...>::n;
	};
/*
	index sequence
*/
template <int...> struct sequence {};

template <class S1, class S2> struct concat_sequence;
template <class S1, class S2> using  concat_sequence_t = typename concat_sequence<S1, S2>::i;

template <int... I1, int... I2>
	struct concat_sequence<sequence<I1...>, sequence<I2...> > {
	  using i = sequence<I1..., (sizeof...(I1)+I2)...>;
	};

/*
	make index sequence
*/
template <int N> 
	struct make_sequence;
template <int N> 
	using  make_sequence_t = typename make_sequence<N>::i;

template <int N>
	struct make_sequence {
	  using i = concat_sequence_t<make_sequence_t<N/2>, make_sequence_t<N - N/2>>;
	};

template <> 
	struct make_sequence<1> { 
		using i = sequence<0>; 
	};
template <> 
	struct make_sequence<0> { 
		using i = sequence<>;
		 };

/*
	sizeof index sequence
*/
template <int... Is>
	constexpr size_t size(sequence<Is...>) {
	  return sizeof...(Is);
	}
/*
	indexes 
*/
template <typename ... ARGS > 
	using  indexes = typename make_sequence< counter<ARGS...>::n >::i;

#define ISOLATE v8::Isolate::GetCurrent()
#define V8ARGS v8::FunctionCallbackInfo<v8::Value>
typedef 
	void (* BINDING_FN_TYPE)(const v8::FunctionCallbackInfo<v8::Value> &);

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
	 	static char * tag;
	 };

template<typename TYPE> 
	 TYPE 
	 	(*link<TYPE>::getter)
	 		(const v8::Local<v8::Value> &) = nullptr;
template<typename TYPE> 
	 v8::Local<v8::Value>
	 	(*link<TYPE>::setter)
	 		(TYPE) = nullptr;
template<typename TYPE> 
	 std::string 
	 	(*link<TYPE>::validator)
	 		(const v8::Local<v8::Value> &) = nullptr;
template<typename TYPE> 
	char * link<TYPE>::tag = nullptr;

template<typename TYPE> 
	struct linker{
		static const char tag [];
	 	static 
	 		TYPE get(const v8::Local<v8::Value> & i ){
	 			return link<TYPE>::getter(i);
	 		}
	 	static 
	 		v8::Local<v8::Value> set(const TYPE i){
	 			if(link<TYPE>::setter != nullptr){
	 				return link<TYPE>::setter(i);
	 			}
	 			return v8::Undefined(ISOLATE);
	 		}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			std::string str;
	 			if(link<TYPE>::validator != nullptr){
	 				str = link<TYPE>::validator(i);
	 			}
	 			if(link<TYPE>::getter == nullptr){
	 				str += "getter function is missing";
	 			}
	 			return str;
	 		}
	 };
template<typename TYPE> 
const char linker<TYPE >::tag [] = "unknown";

template<typename TYPE> 
	struct linker<TYPE *>{
		static const char tag [];
		static 
			TYPE * get(const v8::Local<v8::Value> &i){
				int64_t m = i->NumberValue();
				if(i->IsNull() || (m == 0) ){
					return nullptr;
				}
				return reinterpret_cast<TYPE *>(m);
			};
			static 
				v8::Local<v8::Value> set(const TYPE * data){
					int64_t i = (int64_t)data;
					return v8::Number::New(ISOLATE,i);
				}
	 		static
	 			std::string validate(const v8::Local<v8::Value> &i){
	 			std::string str;
	 			if(!i->IsNumber() && !i->IsNull()){
	 				str += "invalid value, should be a pointer.";
	 			}
	 			return str;
	 		} 
	};
template<typename TYPE> 
const char linker<TYPE  *>::tag [] = "pointer";

template<>
	 struct linker<char *>{
	 	static const char tag [];
	 	static char * get(const v8::Local<v8::Value> &i){
	 		v8::String::Utf8Value str(i);
			return *str;
	 	}
	 	static v8::Local<v8::Value> set(const char * data){
	 		return v8::String::NewFromUtf8(ISOLATE, data);
	 	}
	 	static std::string validate(const v8::Local<v8::Value> &i){
	 		std::string str;
	 		if(!i->IsString()){
	 				str += "invalid value, should be a string.";
	 			}
	 		return str;
	 	}
	 };
const char linker<char *>::tag [] = "char *";
template<>
	 struct linker<const char *>:public linker<char *>{};

template<>
	 struct linker<unsigned char *>{
	 	static const char tag [];
	 	static unsigned char * get(const v8::Local<v8::Value> &i){
			return (unsigned char*) linker<char *>::get(i);
	 	}
	 	static v8::Local<v8::Value> set(const unsigned char * data){
	 		return linker<char *>::set((char *)data);
	 	}
	 	static std::string validate(const v8::Local<v8::Value> &i){
	 		std::string str;
	 		if(!i->IsString()){
	 				str += "invalid value, should be a string.";
	 			}
	 		return str;
	 	}
	 };
const char linker<unsigned char *>::tag [] = "unsigned char *";
template<>
	 struct linker<const unsigned char *>:public linker<unsigned char *>{};

template<>
	 struct linker<std::string>{
	 	static const char tag [];
	 	static std::string get(const v8::Local<v8::Value> &i){
			return linker<char*>::get(i);
	 	}
	 	static v8::Local<v8::Value> set(const std::string str){
	 		return v8::String::NewFromUtf8(ISOLATE, str.data());
	 	}
	 	static std::string validate(const v8::Local<v8::Value> &i){
	 		return linker<char*>::validate(i);
	 	}
	 };
const char linker<std::string>::tag [] = "std::string";

template<>
	 struct linker<double>{
	 	static const char tag [];
	 	static 
	 		double get(const v8::Local<v8::Value> &i){
		 		return i->NumberValue();
		 	}
	 	static 
		 	v8::Local<v8::Value> set(const double i){
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
const char linker<double>::tag [] = "double";
template<>
	 struct linker<const double>:public linker<double>{};

template<>
	 struct linker<float>{
	 	static const char tag [];
	 	static 
	 		float get(const v8::Local<v8::Value> &i){
		 		return i->NumberValue();
		 	}
	 	static 
		 	v8::Local<v8::Value> set(const float i){
		 		return v8::Number::New(ISOLATE,i);
		 	}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			return linker<double>::validate(i);
	 		}
	 };
const char linker<float>::tag [] = "float";
template<>
	 struct linker<const float>:public linker<float>{};


template<>
	 struct linker<int32_t>{
	 	static const char tag [];
	 	static 
	 		int32_t get(const v8::Local<v8::Value> &i){
		 		return i->Int32Value();
		 	}
	 	static 
		 	v8::Local<v8::Value> set(const int32_t i){
		 		return v8::Number::New(ISOLATE,i);
		 	}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			std::string str;
	 			if(!i->IsInt32()){
	 				str += "invalid value, should be a number with a 32bit int value.";
	 			}
	 			return str;
	 		}
	 };
const char linker<int32_t>::tag [] = "int32_t";
template<>
	 struct linker<const int32_t>:public linker<int32_t>{};


template<>
	 struct linker<uint32_t>{
	 	static const char tag [];
	 	static 
	 		uint32_t get(const v8::Local<v8::Value> &i){
		 		return i->Uint32Value();
		 	}
	 	static 
		 	v8::Local<v8::Value> set(const uint32_t i){
		 		return v8::Number::New(ISOLATE,i);
		 	}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){ 
	 			std::string str;
	 			if(!i->IsUint32()){
	 				str += "invalid value, should be a number with an unsigned 32bit int value.";
	 			}
	 			return str;
	 		}
	 };
const char linker<uint32_t>::tag [] = "uint32_t";
template<>
	 struct linker<const uint32_t>:public linker<uint32_t>{};


template<>
	 struct linker<int64_t>{
	 	static const char tag [];
	 	static 
	 		int64_t get(const v8::Local<v8::Value> &i){
		 		return i->IntegerValue();
		 	}
	 	static 
		 	v8::Local<v8::Value> set(const int64_t i){
		 		return v8::Number::New(ISOLATE,i);
		 	}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			return linker<double>::validate(i);
	 		}
	 };	 
const char linker<int64_t>::tag [] = "int64_t";

template<>
	 struct linker<const int64_t>:public linker<int64_t>{};

template<>
	 struct linker<bool>{
	 	static const char tag [];
	 	static 
	 		bool get(const v8::Local<v8::Value> &i){
		 		return i->BooleanValue();
		 	}
	 	static 
		 	v8::Local<v8::Value> set(const bool i){
		 		return v8::Boolean::New(ISOLATE,i);
		 	}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			std::string str;
	 			if(!i->IsBoolean()){
	 				str += "invalid value, should be a boolean.";
	 			}
	 			return str;
	 		}
	 };
const char linker<bool>::tag [] = "boolean";
template<>
	 struct linker<const bool>:public linker<bool>{};


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
	 	static const char tag [];
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
const char linker<void>::tag [] = "void";

template<typename TYPE> 
	struct linker<const TYPE>:linker<TYPE>{};

template<typename FNTYPE , FNTYPE fn ,typename TYPE , int ...Is> 
	struct linker_t : public linker<TYPE>{};

template<typename FNTYPE , FNTYPE fn ,typename R, typename ...ARGS ,int ...Is >
struct linker_t<FNTYPE,fn,R(*)(ARGS...),Is...>{
	typedef R(&rtntype)(ARGS...);
		 static 
	 		rtntype get(const v8::Local<v8::Value> & i){
	 			v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(i);
	 			callback_cache<FNTYPE,fn,Is...>::callback.Reset(ISOLATE,callback);
	 			return linker_t<FNTYPE,fn,R(*)(ARGS...),Is...>::wrapper;
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
							callback_cache<FNTYPE,fn,Is...>::callback);
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

std::string toString(int i){
	std::string str;
	if(i<0){
		str = '-';
		return str + toString(-i);
	}
	do{
		char c =  '0' + (i%10);
		str += c;
		i = i/10;
	}while(i);
	return str;
}

template<typename TYPE, TYPE data> 
	 struct binder_t {
	 	static 
	 		void bind (const V8ARGS & arguments) {
			arguments
				.GetReturnValue()
				.Set(linker<TYPE>::set(data));
			return;
	 		};
	 };

template<typename TYPE, int N, TYPE (&array)[N]> 
	 struct binder_t<TYPE[N],array>{
	 	static 
	 		void bind (const V8ARGS & arguments) {
			  v8::Handle<v8::Array> v8Array = v8::Array::New(arguments.GetIsolate(),N);
			  if (v8Array.IsEmpty()){
			  	return;
			  }
			  for(int i = 0  ;  i < N ; i++){
			  	v8Array->Set(i, linker<TYPE>::set(array[i]));
			  }
			  arguments
				.GetReturnValue()
				.Set(v8Array);
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

		template<int ...Is> static 
			std::string validateArgs (const V8ARGS & arguments ,sequence<Is...>){
				std::string res;
				std::string
				 strs [] = {
				 	linker_t<fntype,fn,ARGS,Is>::validate(arguments[Is])...
				 };
				 int n = counter<ARGS...>::n;
				 for(int i = 0 ; i < n ; i++ )
				 if(!strs[i].empty()){
				 	res = "(#" +  toString(i)+ ")" + strs[i];
				 };
				 return res;
			};
		static 
			std::string tag (){
				int n = counter<ARGS...>::n;
				std::string rtn ;
				rtn+= linker<R>::tag;
				rtn += "(";
				std::string strs [] ={
					std::string(linker<ARGS>::tag)...
				};
				for(int i = 0 ; i <n ; i ++){
					if(i >0){
						rtn += ", ";
					}
					rtn += strs[i];
				}
				rtn += ")";
				return rtn;
			}
		static 
			bool validate (const V8ARGS & arguments){
			int length = arguments.Length(),
			 	n = counter<ARGS...>::n ;
			std::string str;
			v8::Isolate * isolate;

			if(n > length){
				str = "missing "+ toString(n-length) +" arguments.";
			}
			else if(n  < length ){
				str = "to many arguments, overflow by" + toString(length-n) +".";
			} else {
				str = binder_t<fntype,fn>
				 ::validateArgs(arguments,indexes<ARGS...>());
			}

			if(str.empty()){
				return true;
			}
			str =  binder_t<R(ARGS...),fn>::tag()+ "Error:"   + str;
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


}
#endif /* ___PURE__HEDON__BIND___H___ */
