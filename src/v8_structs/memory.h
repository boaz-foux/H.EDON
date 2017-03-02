

template<typename FNTYPE,FNTYPE * fn , int ...INDEX>
	struct hedon_memory {
		static v8::Persistent<v8::Function> callback;
	};

template<typename FNTYPE,FNTYPE * fn , int...INDEX>
	 v8::Persistent<v8::Function>
	 hedon_memory<FNTYPE,fn,INDEX...>::callback;


