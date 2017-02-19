template<typename FNTYPE,FNTYPE * fn > 
	struct hedon_memory {
		static v8::FunctionCallbackInfo<v8::Value> * v8args;
	};

template<typename FNTYPE,FNTYPE * fn > 
	v8::FunctionCallbackInfo<v8::Value> * 
	hedon_memory<FNTYPE,fn>::v8args = NULL;

