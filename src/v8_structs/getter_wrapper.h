
template<typename FNTYPE,FNTYPE * fn ,typename TYPE ,int INDEX,int ...POSTINDEX>  
	struct hedon_getter_wrapper {
		static TYPE get (CONST_ARGUMENTS_REFERENCE){
			return hedon_getter<TYPE>::get(v8args[INDEX]);
		}
	};

template<typename FNTYPE,FNTYPE * fn , int INDEX ,typename R , typename ...VARGS > 
	struct hedon_getter_wrapper<FNTYPE,fn,R(*)(VARGS...),INDEX> {
	typedef R(*fntype)(VARGS...);
	
	static fntype  get (CONST_ARGUMENTS_REFERENCE){
			v8::Local<v8::Function> callback = 
				v8::Local<v8::Function>::Cast(v8args[INDEX]);
			
			hedon_memory<FNTYPE,fn,INDEX>::callback.Reset(
				v8::Isolate::GetCurrent(),
				callback
				);
			return  hedon_getter_wrapper<FNTYPE,fn,R(*)(VARGS...),INDEX>::wrap;
		}

	static R wrap(VARGS... vargs){

			int argc = variadic_counter<VARGS...>::i ;

			v8::Isolate 
				* isolate =  v8::Isolate::GetCurrent();

			v8::Handle<v8::Value> 
				args [] = { 
					hedon_setter<VARGS>::set(isolate,vargs)...  
				};
			 
			v8::Local<v8::Function> callback = 
			v8::Local<v8::Function>::New(isolate, hedon_memory<FNTYPE,fn,INDEX>::callback);
	 
			return hedon_getter<R>::get(
					callback->Call(
						v8::Null(isolate),
						argc,
						args)
					);
		}

	};