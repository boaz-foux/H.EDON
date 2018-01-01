/*
*	MIT License
*	
*	Copyright (c) 2017 Boaz Foux
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


#define BIND(I) binder<decltype(I),I>::bind
				
#define GETTER(C,GET) link<C>::getter = GET
#define SETTER(C,SET) link<C>::setter = SET
#define VALIDATOR(C,VALIDATE) link<C>::validator = VALIDATE
#define TAG(C,TAG) tagger<C>::tag = TAG

#define ISOLATE v8::Isolate::GetCurrent()
#define V8ARGS v8::FunctionCallbackInfo<v8::Value>

#define STRING(TXT) v8::String::New( ISOLATE ,TXT)
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


typedef 
	void (* BINDING_FN_TYPE)(const v8::FunctionCallbackInfo<v8::Value> &);


/*
	caching
*/
template<typename FNTYPE,FNTYPE * fn ,typename TYPE, int ...Is>
	struct cache{
		static TYPE cached;
	};
template<typename FNTYPE,FNTYPE * fn,typename TYPE , int...Is>
	 TYPE  cache<FNTYPE,fn,TYPE,Is...>::cached;


template<typename FNTYPE,FNTYPE * fn , int ...Is>
	struct callback_cache
		: cache<FNTYPE,fn ,v8::Persistent<v8::Function>, Is...> {};

/*
	tagging
*/

template <typename TYPE>
	struct tagger{
	 static std::string tag;
	};
template<typename TYPE>
	std::string tagger<TYPE>::tag  = "unknown";

template <>
	struct tagger<void>{
	 static const char tag [];
	};
const char tagger<void>::tag []= "void";
template <>
	struct tagger<std::string>{
	 static const char tag [];
	};
const char tagger<std::string>::tag [] = "std::string";
template <>
	struct tagger<char *>{
	 static const char tag [];
	};
const char tagger<char *>::tag [] = "char *";
template <>
	struct tagger<unsigned char *>{
	 static const char tag [];
	};
const char tagger<unsigned char *>::tag [] = "unsigned char *";
template <>
	struct tagger<double>{
	 static const char tag [];
	};
const char tagger<double>::tag [] = "double";
template <>
	struct tagger<int32_t>{
	 static const char tag [];
	};
const char tagger<int32_t>::tag [] = "int32";
template <>
	struct tagger<uint32_t>{
	 static const char tag [];
	};
const char tagger<uint32_t>::tag [] = "unsigned int32";
template <>
	struct tagger<int64_t>{
	 static const char tag [];
	};
const char tagger<int64_t>::tag []= "int64";
template <>
	struct tagger<uint64_t>{
	 static const char tag [];
	};
const char tagger<uint64_t>::tag []= "unsigned int64";
template <>
	struct tagger<bool>{
	 static const char tag [];
	};
const char tagger<bool>::tag []= "boolean";


template<typename TYPE >
	struct tag_builder{
		static std::string buildTag(){
			return tagger<TYPE>::tag;
		};
	};

template<typename TYPE >
	struct tag_builder<TYPE *>{
		static std::string buildTag(){
			return std::string("pointer ") + tagger<TYPE>::tag;
		};
	};

template<typename TYPE >
	struct tag_builder<const TYPE>{
		static std::string buildTag(){
			return std::string("const ") + tagger<TYPE>::tag;
		};
	};

template<typename R, typename ...ARGS >
	struct tag_builder<R(ARGS...)>{
		static const std::string tag;
		static std::string buildTag(){
				int n = counter<ARGS...>::n;
				std::string rtn ;
				rtn+= tagger<R>::tag;
				rtn += "(";
				std::string strs [] ={
					std::string(tagger<ARGS>::tag)...
				};
				for(int i = 0 ; i <n ; i ++){
					if(i >0){
						rtn += ", ";
					}
					rtn += strs[i];
				}
				rtn += ")";
				return rtn;
			};
	};
template<typename R, typename ...ARGS >
	struct tag_builder<R(*)(ARGS...)>
		: tag_builder<R(ARGS...)>{};

template <typename TYPE, TYPE t>
	struct tag_use{
	static bool _set;
	static std::string tag;
	};
template<typename TYPE, TYPE t>
	std::string tag_use<TYPE,t>::tag  = "";
template<typename TYPE, TYPE t>
	bool tag_use<TYPE,t>::_set  = false;


/*
	linking
*/

template<typename TYPE> 
	struct link{
	 	static TYPE (*getter)(const v8::Local<v8::Value> &,v8::Isolate *);
	 	static v8::Local<v8::Value> (*setter)(const TYPE,v8::Isolate *);
	 	static std::string (*validator)(const v8::Local<v8::Value> &,v8::Isolate *);
	 };

template<typename TYPE> 
	 TYPE 
	 	(*link<TYPE>::getter)
	 		(const v8::Local<v8::Value> &,v8::Isolate *) = nullptr;
template<typename TYPE> 
	 v8::Local<v8::Value>
	 	(*link<TYPE>::setter)
	 		(TYPE,v8::Isolate *) = nullptr;
template<typename TYPE> 
	 std::string 
	 	(*link<TYPE>::validator)
	 		(const v8::Local<v8::Value> &,v8::Isolate *) = nullptr;

template<typename TYPE> 
	struct linker{
	 	static 
	 		TYPE get(const v8::Local<v8::Value> & i ){
	 			return link<TYPE>::getter(i,ISOLATE);
	 		}
	 	static 
	 		v8::Local<v8::Value> set(const TYPE i){
	 			if(link<TYPE>::setter != nullptr){
	 				return link<TYPE>::setter(i,ISOLATE);
	 			}
	 			return v8::Undefined(ISOLATE);
	 		}
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			std::string str;
	 			if(link<TYPE>::validator != nullptr){
	 				str = link<TYPE>::validator(i,ISOLATE);
	 			}
	 			if(link<TYPE>::getter == nullptr){
	 				str += "getter function is missing";
	 			}
	 			return str;
	 		}
	 };

template<typename TYPE> 
	struct linker<TYPE *>{

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


template<>
	 struct linker<std::string>{

	 	static std::string get(const v8::Local<v8::Value> &i){
	 		v8::String::Utf8Value str(i);
			return std::string(*str);
	 	}
	 	static v8::Local<v8::Value> set(const std::string & str){
	 		return v8::String::NewFromUtf8(ISOLATE, str.data());
	 	}
	 	static std::string validate(const v8::Local<v8::Value> &i){
	 		std::string str;
	 		if(!i->IsString()){
	 				str += "invalid value, should be a string.";
	 			}
	 		return str;
	 	}
	 };


template<>
	 struct linker<char *>{

	 	static char * get(const v8::Local<v8::Value> &i){
	 		v8::String::Utf8Value str(i);
			return *str;
	 	}
	 	static v8::Local<v8::Value> set(const char * data){
	 		return v8::String::NewFromUtf8(ISOLATE, data);
	 	}
	 	static std::string validate(const v8::Local<v8::Value> &i){
	 		return linker<std::string>::validate(i);
	 	}
	 };

template<>
	 struct linker<const char *>:public linker<char *>{};

template<>
	 struct linker<unsigned char *>{

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

template<>
	 struct linker<const unsigned char *>:public linker<unsigned char *>{};


template<>
	 struct linker<double>{

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
template<>
	 struct linker<const double>:public linker<double>{};

template<>
	 struct linker<float>{

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
template<>
	 struct linker<const float>:public linker<float>{};


template<>
	 struct linker<int32_t>{
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
template<>
	 struct linker<const int32_t>:public linker<int32_t>{};


template<>
	 struct linker<uint32_t>{

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
template<>
	 struct linker<const uint32_t>:public linker<uint32_t>{};


template<>
	 struct linker<int64_t>{

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
template<>
	 struct linker<const int64_t>:public linker<int64_t>{};

template<>
	 struct linker<bool>{

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
template<>
	 struct linker<const bool>:public linker<bool>{};


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
template<typename TYPE> 
	struct linker<const TYPE>:linker<TYPE>{};


template<typename FNTYPE , FNTYPE fn ,typename TYPE , int ...Is>
	struct linker_t : public linker<TYPE>{};

template<typename FNTYPE , FNTYPE fn ,int ...Is >
struct linker_t<FNTYPE,fn,std::string,Is...>{
	static std::string
		get(const v8::Local<v8::Value> & i){
			return cache<FNTYPE,fn,std::string,Is...>::cached
			 		= linker<std::string>::get(i);
		};
	static
	 	std::string validate(const v8::Local<v8::Value> &){
	 			return std::string();
	 		}
};

/*
	replacing
*/
template<typename TYPE>
	struct replace_suggestion{
		typedef TYPE suggestion;
	};
template<>
	struct replace_suggestion<void>{
		typedef bool suggestion;
	};

template<>
	struct replace_suggestion<char *>{
		typedef const std::string suggestion;
	};
template<>
	struct replace_suggestion<const char *>{
		typedef const std::string suggestion;
	};


template<typename SRC, typename DEST>
	struct replacer;

template<>
	struct replacer<void,bool>{
		static bool replace (bool i = false){ return false; }
	};

template<>
	struct replacer<bool, void>{
		static void replace (bool i = false){ return ; }
	};


template<typename SRC>
	struct replacer<SRC , const std::string >{
		static  std::string replace (SRC  src){
		 return std::string(src);
		}
	};

template<typename DEST>
	struct replacer<const std::string ,DEST>{
		static DEST replace (const std::string & src){
		 return (DEST)src.data();
		}
	};

template<typename SRC>
	struct replacer<SRC,SRC>{
		static SRC replace (SRC src){ return src; }
	};

template<typename FNTYPE,FNTYPE * fn ,typename SRC, typename DEST,int ...Is>
	struct replacer_cache :replacer<SRC,DEST> {};

template<typename FNTYPE,FNTYPE * fn , typename DEST,int ...Is>
	struct replacer_cache<FNTYPE,fn ,const std::string,DEST, Is...>{
		static DEST replace (const std::string & src){
			cache<FNTYPE,fn ,std::string, Is...>::cached =  "" + src ;
		 return replacer<const std::string ,DEST>::replace(
		 	cache<FNTYPE,fn ,std::string, Is...>::cached
		 	);
		}
	};

template<typename FNTYPE,FNTYPE * fn , typename SRC,int ...Is>
	struct replacer_cache<FNTYPE,fn ,SRC,const std::string, Is...>{
		static  std::string replace (SRC  src){
		 return cache<FNTYPE,fn ,std::string, Is...>::cached 
		 = replacer<SRC ,const std::string >::replace(
		 	 src
		 	);
		}
	};

template< typename M>
	using suggest =
		 typename replace_suggestion<M>::suggestion;

/*
	linker
*/
template<typename FNTYPE , FNTYPE fn ,typename R, typename ...ARGS ,int ...Is >
struct linker_t<FNTYPE,fn,R(*)(ARGS...),Is...>{
	typedef R(&rtntype)(ARGS...);
		 static 
	 		rtntype get(const v8::Local<v8::Value> & i){
	 			v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(i);
	 			callback_cache<FNTYPE,fn,Is...>::cached.Reset(ISOLATE,callback);
	 			return linker_t<FNTYPE,fn,R(*)(ARGS...),Is...>::wrapper;
	 		}
	 	static 
	 		R wrapper (ARGS...args){
	 		return	replacer_cache< FNTYPE,fn,suggest<R>,R ,-1>::replace(
	 					call(replacer< ARGS , suggest<ARGS> > ::replace(args)...)
	 				);
	 		}


		static suggest<R> call(suggest<ARGS> ... args ){
			int argc = counter<ARGS...>::n ;
	 			v8::Handle<v8::Value> argv [] = { 
					linker<suggest<ARGS>>::set(args)...
				};
				v8::Local<v8::Function> 
					callback = v8
						::Local<v8::Function>
						::New(ISOLATE,
							callback_cache<FNTYPE,fn,Is...>::cached);
				return linker< suggest<R> >::get(callback->Call(
											v8::Null(ISOLATE),
											argc,
											argv));

		};
	 	static 
	 		std::string validate(const v8::Local<v8::Value> &i){
	 			if(!i->IsFunction()){
	 				return "invalid value, should be a function.";
	 			}
	 			return "";
	 		}
};

std::string toString(int i){
	std::string str;
	if(i<0){
		str = '-';
		i = -i;
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
template<typename R, typename ...ARGS, R(&&fn)(ARGS...)>
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
				return tag_use<R(ARGS...),fn>::tag;
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
			str =  tag_use<R(ARGS...),fn>::tag + "Error:"   + str;
			isolate =  arguments.GetIsolate();
			isolate->ThrowException(
				v8::String::NewFromUtf8(isolate,str.data())
				);
			return false;
			};
	};

template<typename TYPE, TYPE data> struct binder :public binder_t<TYPE,data> {};

template< typename R ,typename ...ARGS, R(&&fn)(ARGS...)>
struct binder<R(ARGS...),fn> {
		typedef suggest<R>(fntype)(suggest<ARGS> ...);

		static void bind (const V8ARGS & arguments) {
			if(!tag_use<fntype,binder<R(ARGS...),fn>::wrap>::_set){
				tag_use<fntype,binder<R(ARGS...),fn>::wrap>::_set = true;
				tag_use<fntype,binder<R(ARGS...),fn>::wrap>::tag =
				tag_builder<R(ARGS...)>::buildTag();
			};
			return binder_t<fntype,binder<R(ARGS...),fn>::wrap>::bind(arguments);
		};

		static suggest<R> wrap(suggest<ARGS> ... args){
			return binder<R(ARGS...),fn>::call(args... , indexes<ARGS...>() );
		};

		template<int ...Is>
		static suggest<R> call(suggest<ARGS> ... args ,sequence<Is...>){
			return replacer_cache< R(ARGS...),fn,R, suggest<R> ,-1>::replace(
				fn(
					replacer_cache< R(ARGS...),fn, suggest<ARGS> ,ARGS,Is >::replace(args)...
					)
				 );
		};
};
template< typename ...ARGS, void(&&fn)(ARGS...)>
struct binder<void(ARGS...),fn> {
		template< typename M>
			using suggest =
				 typename replace_suggestion<M>::suggestion;
		typedef suggest<void>(fntype)( ARGS ...);

		static void bind (const V8ARGS & arguments) {
			return binder<fntype,binder<void(ARGS...),fn>::wrap>::bind(arguments);
		};

		static suggest<void> wrap( ARGS ... args){
			fn( args...);
			return replacer< void,suggest<void> >::replace();
		};
};



}
#endif /* ___PURE__HEDON__BIND___H___ */
