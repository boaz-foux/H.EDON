/*
*	MIT License
*	
*	Copyright (c) 2018 Boaz Foux
*	
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*	
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*	
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*/

#ifndef ___PURE__HEDON__BIND___H___
#define ___PURE__HEDON__BIND___H___

#include <string>

namespace HEDON {

/* should be refactored*/
#define GETTER(C,GET) Link<C>::getter = GET
#define SETTER(C,SET) Link<C>::setter = SET
#define VALIDATOR(C,VALIDATE) Link<C>::validator = VALIDATE
#define TAG(C,TAG) Link<C>::tag = TAG
/* should be refactored*/

#define ISOLATE \
	v8::Isolate::GetCurrent()

#define JS_BIND_CALLBACK_TYPE \
		v8::FunctionCallback  

#define JS_VALUE \
		v8::Local<v8::Value>

#define JS_EXPORTS \
		v8::Handle<v8::Object>

#define JS_ARGS \
	v8::FunctionCallbackInfo<v8::Value>

#define JS_OBJECT_TYPE \
		v8::Local<v8::Object>

#define JS_ARRAY(N_NUMBER_OF_ELEMENTS) \
		v8::Array::New(ISOLATE, N_NUMBER_OF_ELEMENTS );

#define JS_ARRAY_TYPE \
		v8::Local<v8::Array>

#define JS_NUMBER(NUMBER_VALUE) \
		v8::Number::New(ISOLATE,NUMBER_VALUE)

#define JS_BOOLEAN(BOOLEAN_VALUE) \
	v8::Boolean::New(ISOLATE,BOOLEAN_VALUE)

#define JS_STRING(TXT_VALUE) \
	v8::String::NewFromUtf8(ISOLATE ,TXT_VALUE)

#define JS_NULL \
	v8::Null(ISOLATE)

#define JS_UNDEFINED \
	v8::Undefined(ISOLATE)

#define JS_FUNCTION_TEMPLATE \
			v8::Local<v8::FunctionTemplate> 

#define JS_NEW_FUNCTION_TEMPLATE(CALLBACK) \
			v8::FunctionTemplate::New(ISOLATE,CALLBACK)

#define JS_RETURN(ARGUMENTS, VALUE) ARGUMENTS.GetReturnValue().Set(VALUE)

#define JS_THROW(ERROR_TXT_VALUE) \
    		ISOLATE->ThrowException( \
          		v8::Exception::Error( \
               		JS_STRING( ERROR_TXT_VALUE ) \
            	) \
         	)

#define BIND(I) \
	Binder<decltype(I),I>::bind

#define EXPORT(EXP,I) \
	Binder<decltype(I),I>::exporter(EXP,#I)

/*
	Counter
*/ 
template< typename ...TYPES> 
	class Counter {};

template<>
	class Counter<> {
	public:
	    static const int n = 0;
	};

template<typename T, typename... TYPES>
	class Counter<T, TYPES...> {
	public:
	    static const int n = 1 + Counter<TYPES...>::n;
	};
/*
	index Sequence
*/
template <int...> class Sequence {};

template <class S1, class S2> 
		class ConcatSequence;
template <class S1, class S2> 
		using ConcatSequenceTemp = typename ConcatSequence<S1, S2>::i;

template <int... I1, int... I2>
	class ConcatSequence<	Sequence<I1...>,	Sequence<I2...>		> {
	public:
	  using i = Sequence<I1..., (sizeof...(I1)+I2)...>;
	};

/*
	make index Sequence
*/
template <int N> 
	class MakeSequence;
	
template <int N> 
	using  MakeSequenceTemp = typename MakeSequence<N>::i;

template <int N>
	class MakeSequence {
	public:
	  using i = ConcatSequenceTemp<MakeSequenceTemp<N/2>, MakeSequenceTemp<N - N/2>>;
	};

template <> 
	class MakeSequence<1> { 
	public:
		using i = Sequence<0>; 
	};
template <> 
	class MakeSequence<0> {
	public: 
		using i = Sequence<>;
	};

/*
	sizeof index Sequence
*/
template <int... Is>
	constexpr size_t size(Sequence<Is...>) {
	  return sizeof...(Is);
	}
/*
	Indexing 
*/
template <typename ... ARGS > 
	using  Indexes = typename MakeSequence< Counter<ARGS...>::n >::i;



/*
	caching
*/
template<typename FNTYPE,FNTYPE * fn ,typename TYPE, int ...Is>
	class Cache{
		public: 
		static TYPE cached;
	};
template<typename FNTYPE,FNTYPE * fn,typename TYPE , int...Is>
	 TYPE  Cache<FNTYPE,fn,TYPE,Is...>::cached;


template<typename FNTYPE,FNTYPE * fn , int ...Is>
	class CallbackCache
		: public Cache<FNTYPE,fn ,v8::Persistent<v8::Function>, Is...> {};

/*
	linking
*/

template<typename TYPE> 
	class Link {
	public:
	 	static TYPE (*getter)(const JS_VALUE &,v8::Isolate *);
	 	static JS_VALUE (*setter)(const TYPE,v8::Isolate *);
	 	static std::string (*validator)(const JS_VALUE &,v8::Isolate *);
	 	static char * tag;
	 };

template<typename TYPE> 
	 TYPE 
	 	(*Link<TYPE>::getter)
	 		(const JS_VALUE &,v8::Isolate *) = nullptr;

template<typename TYPE> 
	 JS_VALUE
	 	(*Link<TYPE>::setter)
	 		(TYPE,v8::Isolate *) = nullptr;

template<typename TYPE> 
	 std::string 
	 	(*Link<TYPE>::validator)
	 		(const JS_VALUE &,v8::Isolate *) = nullptr;

template<typename TYPE> 
	char * Link<TYPE>::tag = nullptr;

template<typename TYPE> 
	class Linker{
	public:
		static const char tag [];
	 	static 
	 		TYPE get(const JS_VALUE & i ){
	 			return Link<TYPE>::getter(i,ISOLATE);
	 		}
	 	static 
	 		JS_VALUE set(const TYPE i){
	 			if(Link<TYPE>::setter != nullptr){
	 				return Link<TYPE>::setter(i,ISOLATE);
	 			}
	 			return JS_UNDEFINED;
	 		}
	 	static 
	 		std::string validate(const JS_VALUE &i){
	 			std::string str;
	 			if(Link<TYPE>::validator != nullptr){
	 				str = Link<TYPE>::validator(i,ISOLATE);
	 			}
	 			if(Link<TYPE>::getter == nullptr){
	 				str += "getter function is missing";
	 			}
	 			return str;
	 		}
	 };
template<typename TYPE> 
const char Linker<TYPE >::tag [] = "unknown";

template<typename TYPE> 
	class Linker<TYPE *>{
	public:
		static const char tag [];
		static 
			TYPE * get(const JS_VALUE &i){
				int64_t m = i->NumberValue();
				if(i->IsNull() || (m == 0) ){
					return nullptr;
				}
				return reinterpret_cast<TYPE *>(m);
			};
			static 
				JS_VALUE set(const TYPE * data){
					int64_t i = (int64_t)data;
					return JS_NUMBER(i);
				}
	 		static
	 			std::string validate(const JS_VALUE &i){
	 			std::string str;
	 			if(!i->IsNumber() && !i->IsNull()){
	 				str += "invalid value, should be a pointer.";
	 			}
	 			return str;
	 		} 
	};
template<typename TYPE> 
const char Linker<TYPE  *>::tag [] = "pointer";

template<>
	 class Linker<std::string>{
	 public:
	 	static const char tag [];
	 	static std::string get(const JS_VALUE &i){
	 		v8::String::Utf8Value str(ISOLATE, i);
			return std::string(*str);
	 	}
	 	static JS_VALUE set(const std::string & str){
	 		return JS_STRING(str.data());
	 	}
	 	static std::string validate(const JS_VALUE &i){
	 		std::string str;
	 		if(!i->IsString()){
	 				str += "invalid value, should be a string.";
	 			}
	 		return str;
	 	}
	 };
const char Linker<std::string>::tag [] = "std::string";


template<>
	 class Linker<char *>{
	 public:
	 	static const char tag [];
	 	static char * get(const JS_VALUE &i){
	 		v8::String::Utf8Value str(ISOLATE,i);
			return *str;
	 	}
	 	static JS_VALUE set(const char * data){
	 		return JS_STRING(data);
	 	}
	 	static std::string validate(const JS_VALUE &i){
	 		return Linker<std::string>::validate(i);
	 	}
	 };
const char Linker<char *>::tag [] = "char *";

template<>
	 class Linker<unsigned char *>{
	 public:
	 	static const char tag [];
	 	static unsigned char * get(const JS_VALUE &i){
			return (unsigned char*) Linker<char *>::get(i);
	 	}
	 	static JS_VALUE set(const unsigned char * data){
	 		return Linker<char *>::set((char *)data);
	 	}
	 	static std::string validate(const JS_VALUE &i){
	 		std::string str;
	 		if(!i->IsString()){
	 				str += "invalid value, should be a string.";
	 			}
	 		return str;
	 	}
	 };
const char Linker<unsigned char *>::tag [] = "unsigned char *";


template<>
	 class Linker<double>{
	 public:
	 	static const char tag [];
	 	static 
	 		double get(const JS_VALUE &i){
		 		return i->NumberValue();
		 	}
	 	static 
		 	JS_VALUE set(const double i){
		 		return JS_NUMBER(i);
		 	}
	 	static 
	 		std::string validate(const JS_VALUE &i){
	 			std::string str;
	 			if(!i->IsNumber()){
	 				str += "invalid value, should be a number.";
	 			}
	 			return str;
	 		}
	 };
const char Linker<double>::tag [] = "double";


template<>
	 class Linker<float>{
	 public:
	 	static const char tag [];
	 	static 
	 		float get(const JS_VALUE &i){
		 		return i->NumberValue();
		 	}
	 	static 
		 	JS_VALUE set(const float i){
		 		return JS_NUMBER(i);
		 	}
	 	static 
	 		std::string validate(const JS_VALUE &i){
	 			return Linker<double>::validate(i);
	 		}
	 };
const char Linker<float>::tag [] = "float";
template<>
	 class Linker<const float>:public Linker<float>{};


template<>
	 class Linker<int32_t>{
	 public:
	 	static const char tag [];
	 	static 
	 		int32_t get(const JS_VALUE &i){
		 		return i->Int32Value();
		 	}
	 	static 
		 	JS_VALUE set(const int32_t i){
		 		return JS_NUMBER(i);
		 	}
	 	static 
	 		std::string validate(const JS_VALUE &i){
	 			std::string str;
	 			if(!i->IsInt32()){
	 				str += "invalid value, should be a number with a 32bit int value.";
	 			}
	 			return str;
	 		}
	 };
const char Linker<int32_t>::tag [] = "int32_t";


template<>
	 class Linker<uint32_t>{
	 public:
	 	static const char tag [];
	 	static 
	 		uint32_t get(const JS_VALUE &i){
		 		return i->Uint32Value();
		 	}
	 	static 
		 	JS_VALUE set(const uint32_t i){
		 		return JS_NUMBER(i);
		 	}
	 	static 
	 		std::string validate(const JS_VALUE &i){ 
	 			std::string str;
	 			if(!i->IsUint32()){
	 				str += "invalid value, should be a number with an unsigned 32bit int value.";
	 			}
	 			return str;
	 		}
	 };
const char Linker<uint32_t>::tag [] = "uint32_t";


template<>
	 class Linker<int64_t>{
	 public:
	 	static const char tag [];
	 	static 
	 		int64_t get(const JS_VALUE &i){
		 		return i->IntegerValue();
		 	}
	 	static 
		 	JS_VALUE set(const int64_t i){
		 		return JS_NUMBER(i);
		 	}
	 	static 
	 		std::string validate(const JS_VALUE &i){
	 			return Linker<double>::validate(i);
	 		}
	 };	 
const char Linker<int64_t>::tag [] = "int64_t";



template<>
	 class Linker<bool>{
	 public:
	 	static const char tag [];
	 	static 
	 		bool get(const JS_VALUE &i){
		 		return i->BooleanValue();
		 	}
	 	static 
		 	JS_VALUE set(const bool i){
		 		return JS_BOOLEAN(i);
		 	}
	 	static 
	 		std::string validate(const JS_VALUE &i){
	 			std::string str;
	 			if(!i->IsBoolean()){
	 				str += "invalid value, should be a boolean.";
	 			}
	 			return str;
	 		}
	 };
const char Linker<bool>::tag [] = "boolean";

template<>
	 class Linker<void>{
	 public:
	 	static const char tag [];
	 	static 
	 		void get(const JS_VALUE &){
		 		return;
		 	}
	 	static 
		 	JS_VALUE set( void ){
		 		return v8::Null(ISOLATE);
		 	}
	 	static 
	 		std::string validate(const JS_VALUE &){
	 			return std::string();
	 		}
	 };
const char Linker<void>::tag [] = "void";



template<typename TYPE,int N> 
	 class Linker<TYPE[N]>{
	 public:
	 	static const char tag [];
	 	static 
	 		TYPE * get(const JS_VALUE & i){
	 			return nullptr;
		 	}
	 	static 
		 	JS_VALUE set( const TYPE array [N]){
	 			JS_ARRAY_TYPE vArray = JS_ARRAY(N);
				  if (vArray.IsEmpty()){
				  	return vArray;
				  }
				  for(int i = 0  ;  i < N ; i++){
				  	vArray->Set(i, Linker<TYPE>::set(array[i]));
				  }
		 		return vArray;
		 	}
	 	static 
	 		std::string validate(const JS_VALUE & i){
	 			return std::string("invalid by default!");
	 		}
	 };

template<typename TYPE,int N> 	 
const char Linker<TYPE[N]>::tag [] = "array";




template<typename TYPE> 
	class Linker<const TYPE>: public Linker<TYPE>{};



template<typename FNTYPE , FNTYPE fn ,typename TYPE , int ...Is> 
	class LinkerCore : public Linker<TYPE>{};

template<typename FNTYPE , FNTYPE fn ,int ...Is >
class LinkerCore<FNTYPE,fn,std::string,Is...>{
public:
	static std::string 
		get(const JS_VALUE & i){ 
			return Cache<FNTYPE,fn,std::string,Is...>::cached 
			 		= Linker<std::string>::get(i);
		};
	static 
	 	std::string validate(const JS_VALUE &){
	 			return std::string();
	 		}
};

/*
	replacing
*/ 
template<typename TYPE> 
	class ReplaceSuggestion{
	public:
		typedef TYPE suggestion;
	};
template<> 
	class ReplaceSuggestion<void>{
	public:
		typedef bool suggestion;
	};

template<> 
	class ReplaceSuggestion<char *>{
	public:
		typedef const std::string suggestion;
	};
template<> 
	class ReplaceSuggestion<const char *>{
	public:
		typedef const std::string suggestion;
	};


template<typename SRC, typename DEST> 
	class Replacer;

template<> 
	class Replacer<void,bool>{
	public:
		static bool replace (bool i = false){ return false; }
	};

template<> 
	class Replacer<bool, void>{
	public:
		static void replace (bool i = false){ return ; }
	};


template<typename SRC> 
	class Replacer<SRC , const std::string >{
	public:
		static  std::string replace (SRC  src){
		 return std::string(src); 
		}
	};

template<typename DEST> 
	class Replacer<const std::string ,DEST>{
	public:
		static DEST replace (const std::string & src){
		 return (DEST)src.data(); 
		}
	};

template<typename SRC> 
	class Replacer<SRC,SRC>{
			public:
		static SRC replace (SRC src){ return src; }
	};

template<typename FNTYPE,FNTYPE * fn ,typename SRC, typename DEST,int ...Is>
	class ReplacerCache :public Replacer<SRC,DEST> {};

template<typename FNTYPE,FNTYPE * fn , typename DEST,int ...Is>
	class ReplacerCache<FNTYPE,fn ,const std::string,DEST, Is...>{
			public:
		static DEST replace (const std::string & src){
		 return Replacer<const std::string ,DEST>::replace(
		 	Cache<FNTYPE,fn ,std::string, Is...>::cached = src
		 	); 
		}
	};

template<typename FNTYPE,FNTYPE * fn , typename SRC,int ...Is>
	class ReplacerCache<FNTYPE,fn ,SRC,const std::string, Is...>{
	public:
		static  std::string replace (SRC  src){
		 return Replacer<SRC ,const std::string >::replace(
		 	Cache<FNTYPE,fn ,SRC, Is...>::cached = src
		 	); 
		}
	};

template< typename M>
	using Suggest =
		 typename ReplaceSuggestion<M>::suggestion;

/*
	linker
*/
template<typename FNTYPE , FNTYPE fn ,typename R, typename ...ARGS ,int ...Is >
class LinkerCore<FNTYPE,fn,R(*)(ARGS...),Is...>{
		public:
	typedef R(&rtntype)(ARGS...);
		 static 
	 		rtntype get(const JS_VALUE & i){
	 			v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(i);
	 			CallbackCache<FNTYPE,fn,Is...>::cached.Reset(ISOLATE,callback);
	 			return LinkerCore<FNTYPE,fn,R(*)(ARGS...),Is...>::wrapper;
	 		}
	 	static 
	 		R wrapper (ARGS...args){
	 			//Indexes<ARGS...>()
	 		return	ReplacerCache< FNTYPE,fn,Suggest<R>,R ,-1>::replace(
	 					call(Replacer< ARGS , Suggest<ARGS> > ::replace(args)...)
	 				);
	 		}


		static Suggest<R> call(Suggest<ARGS> ... args ){
			int argc = Counter<ARGS...>::n ;
	 			v8::Handle<v8::Value> argv [] = { 
					Linker<Suggest<ARGS>>::set(args)...  
				};
				v8::Local<v8::Function> 
					callback = v8
						::Local<v8::Function>
						::New(ISOLATE,
							CallbackCache<FNTYPE,fn,Is...>::cached);
				return Linker< Suggest<R> >::get(callback->Call(
											v8::Null(ISOLATE),
											argc,
											argv));

		};
	 	static 
	 		std::string validate(const JS_VALUE &){
	 			return std::string();
	 		}
};




template<typename TYPE, TYPE data> 
	 class BinderCore {
	 public:
	 	static 
	 		void bind (const JS_ARGS & arguments) {
			JS_RETURN(arguments, Linker<TYPE>::set(data));
			return;
	 		};
	 };

template<typename TYPE, int N, TYPE (&array)[N]> 
	 class BinderCore<TYPE[N],array>{
	 public:
	 	static JS_VALUE generate(){
	 		JS_ARRAY_TYPE vArray = JS_ARRAY(N);
			  if (vArray.IsEmpty()){
			  	return vArray;
			  }
			  for(int i = 0  ;  i < N ; i++){
			  	vArray->Set(i, Linker<TYPE>::set(array[i]));
			  }
	 		return vArray;
	 	};

	 	static 
	 		void bind (const JS_ARGS & arguments) {
	 		  JS_VALUE value = BinderCore<TYPE[N],array>::generate(); 
			  JS_RETURN(arguments, value);
			  return;
	 		};
	};
template<typename R, typename ...ARGS, R(&&fn)(ARGS...)>
	class BinderCore<R(ARGS...),fn>{
		 public:
		typedef R(fntype)(ARGS...);
		static 
			void bind (const JS_ARGS & arguments) {
				if(!BinderCore<fntype,fn>::validate(arguments)){
					return;
				}				
				R value = 
				 BinderCore<fntype,fn>
				 ::invoke(arguments,Indexes<ARGS...>());
				JS_RETURN(arguments, Linker<R>::set(value));
			};
		template<int ...Is> static 
			R invoke (const JS_ARGS & arguments ,Sequence<Is...>){
				return fn( LinkerCore<fntype,fn,ARGS,Is>::get(arguments[Is])...);
			};

		template<int ...Is> static 
			std::string validateArgs (const JS_ARGS & arguments ,Sequence<Is...>){
				std::string res;
				std::string
				 strs [] = {
				 	LinkerCore<fntype,fn,ARGS,Is>::validate(arguments[Is])...
				 };
				 int n = Counter<ARGS...>::n;
				 for(int i = 0 ; i < n ; i++ )
				 if(!strs[i].empty()){
				 	res = "(#" +  std::to_string(i)+ ")" + strs[i];
				 };
				 return res;
			};
		static 
			std::string tag (){
				int n = Counter<ARGS...>::n;
				std::string rtn ;
				rtn+= Linker<R>::tag;
				rtn += "(";
				std::string strs [] ={
					std::string(Linker<ARGS>::tag)...
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
			bool validate (const JS_ARGS & arguments){
			int length = arguments.Length(),
			 	n = Counter<ARGS...>::n ;
			std::string str;

			if(n > length){
				str = "missing "+ std::to_string(n-length) +" arguments.";
			}
			else if(n  < length ){
				str = "to many arguments, overflow by" + std::to_string(length-n) +".";
			} else {
				str = BinderCore<fntype,fn>
				 ::validateArgs(arguments,Indexes<ARGS...>());
			}

			if(str.empty()){
				return true;
			}
			str =  BinderCore<R(ARGS...),fn>::tag()+ "Error:"   + str;
				JS_THROW(str.data());
			return false;
			};
	};

template<typename TYPE, TYPE data> class Binder:public BinderCore<TYPE,data> {
public:
	static void exporter (const JS_OBJECT_TYPE & exportObj ,const char * key) {
		JS_VALUE vKey = JS_STRING(key);
		exportObj->Set(vKey,Linker<TYPE>::set(data));
	}
};

template< typename R ,typename ...ARGS, R(&&fn)(ARGS...)>
class Binder <R(ARGS...),fn> {
		public:
		typedef Suggest<R>(fntype)(Suggest<ARGS> ...);

		static void exporter (const JS_OBJECT_TYPE & exportObj ,const char * key) {
			NODE_SET_METHOD(exportObj,
				key,
				Binder<R(ARGS...),fn>::bind);
		};

		static void bind (const JS_ARGS & arguments) {
			return BinderCore<fntype,Binder<R(ARGS...),fn>::wrap>::bind(arguments);
		};
		
		static Suggest<R> wrap(Suggest<ARGS> ... args){
			return Binder<R(ARGS...),fn>::call(args... , Indexes<ARGS...>() );
		};
		template<int ...Is> 
		static Suggest<R> call(Suggest<ARGS> ... args ,Sequence<Is...>){
			return ReplacerCache< R(ARGS...),fn,R, Suggest<R> ,-1>::replace(
				fn( 
					ReplacerCache< R(ARGS...),fn, Suggest<ARGS> ,ARGS,Is >::replace(args)...
					)
				 );
		};
};
template< typename ...ARGS, void(&&fn)(ARGS...)>
class Binder<void(ARGS...),fn> {
		public:
		template< typename M>
			using suggest =
				 typename ReplaceSuggestion<M>::suggestion;

		typedef Suggest<void>(fntype)(Suggest<ARGS> ...);

		static void exporter (const JS_OBJECT_TYPE & exportObj ,const char * key) {
			NODE_SET_METHOD(exportObj,
				key,
				Binder<void(ARGS...),fn>::bind);
		};

		static void bind (const JS_ARGS & arguments) {
			return Binder<fntype,Binder<void(ARGS...),fn>::wrap>::bind(arguments);
		};
		

		static Suggest<void> wrap(Suggest<ARGS> ... args){
			fn( Replacer< Suggest<ARGS> ,ARGS  >::replace(args)...);
			return Replacer< void,Suggest<void> >::replace();
		};
};



}
#endif /* ___PURE__HEDON__BIND___H___ */
