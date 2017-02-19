
template<typename FNTYPE,FNTYPE * fn ,int INDEX ,typename TYPE>  
	struct hedon_getter_wrapper {
		static TYPE get (CONST_ARGUMENTS_REFERENCE){
			return hedon_getter<TYPE>::get(v8args[INDEX]);
		}

	};

template<typename FNTYPE,FNTYPE * fn ,int INDEX ,typename R , typename ...VARGS > 
	struct hedon_getter_wrapper<FNTYPE,fn ,INDEX,R(*)(VARGS...)> {
	typedef R(*fntype)(VARGS...);
	
	static fntype  get (CONST_ARGUMENTS_REFERENCE){

				hedon_memory
				<FNTYPE,fn>
				::v8args = 
					(v8::FunctionCallbackInfo<v8::Value> *) 
					&v8args;

			return  hedon_getter_wrapper<FNTYPE,fn,INDEX,R(*)(VARGS...)>::wrap;
		}

		static R wrap(VARGS... vargs){

			int argc = variadic_counter<VARGS...>::i ;
			v8::FunctionCallbackInfo<v8::Value> *
			 v8args = hedon_memory
						<FNTYPE,fn>
						::v8args;

			v8::Isolate 
				* isolate = v8args->GetIsolate();

			v8::Handle<v8::Value> 
				args [] = { 
					hedon_setter<VARGS>::set(isolate,vargs)...  
				};
			v8::Local<v8::Function> 
				callback = v8::Local<v8::Function>::Cast((*v8args)[INDEX]);

			 
			return hedon_getter<R>::get(
					callback->Call(
						v8::Null(isolate),
						argc,
						args)
					);
		}

	};