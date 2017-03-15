

template<typename FNTYPE,FNTYPE * fn , int ...INDEX>
	struct callback_cache {
		static v8::Persistent<v8::Function> callback;
	};

template<typename FNTYPE,FNTYPE * fn , int...INDEX>
	 v8::Persistent<v8::Function>
	 callback_cache<FNTYPE,fn,INDEX...>::callback;



	 