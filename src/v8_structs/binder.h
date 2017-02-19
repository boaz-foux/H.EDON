template<typename RTYPE ,typename FNTYPE,FNTYPE fn > 
	struct hedon_binder {
		static void bind(CONST_ARGUMENTS_REFERENCE){
			RTYPE _return;
			char * msg = hedon_validator<FNTYPE>::check(v8args);// str.data()
			v8::Isolate * isolate =  v8args.GetIsolate();
			if(msg){
				isolate->ThrowException(v8::String::NewFromUtf8(isolate,msg));
			}
			_return = hedon_caller<0,FNTYPE,fn>::bind(v8args);
			v8args.GetReturnValue().Set(hedon_setter<RTYPE>::set(isolate,_return) );
		};
};

template<typename FNTYPE,FNTYPE fn > 
	struct hedon_binder<void,FNTYPE,fn> { 
		static void bind(CONST_ARGUMENTS_REFERENCE){
			char * msg = hedon_validator<FNTYPE>::check(v8args);
			v8::Isolate * isolate =  v8args.GetIsolate();
			if(msg){
				isolate->ThrowException(v8::String::NewFromUtf8(isolate,msg));
			}
			 hedon_caller<0,FNTYPE,fn>::bind(v8args);
		};
};