template< int INDEX,
			typename FNTYPE , FNTYPE * fn,
			typename ... TARGS	>
	struct hedon_caller{};


template< int INDEX,
		typename R , R(*fn)()	>
struct hedon_caller<INDEX ,R(),fn>{
	static R bind (CONST_ARGUMENTS_REFERENCE){
			return fn();
		};
};

template< int INDEX,
		typename R, typename V, 
		R(*fn)(V)	>
struct hedon_caller<INDEX ,R(V),fn>{
	typedef R(fntype)(V);
	static R bind (CONST_ARGUMENTS_REFERENCE ){
			return fn(hedon_getter_wrapper<fntype,fn,V,INDEX>::get(v8args));
		};
};


template< int INDEX,
		typename R, typename V0, typename V1,
		 R(*fn)(V0,V1)>
struct hedon_caller<INDEX ,R(V0,V1),fn>{
	typedef R(fntype)(V0,V1);
	static R bind (CONST_ARGUMENTS_REFERENCE ){
			return 
				fn(hedon_getter_wrapper<fntype,fn,V0,INDEX>::get(v8args),
					hedon_getter_wrapper<fntype,fn,V1,INDEX +1>::get(v8args));
		};
};


template< int INDEX,
		typename R, typename V0, typename V1, typename V2,
		 R(*fn)(V0,V1,V2)>
struct hedon_caller<INDEX ,R(V0,V1,V2),fn>{
	static R bind (CONST_ARGUMENTS_REFERENCE ){
		typedef R(fntype)(V0,V1,V2);
			return
				fn(hedon_getter_wrapper<fntype,fn,V0,INDEX>::get(v8args),
					hedon_getter_wrapper<fntype,fn,V1,INDEX +1>::get(v8args),
					hedon_getter_wrapper<fntype,fn,V2,INDEX +2>::get(v8args));
		};
};

template< int INDEX,
		typename R , 
		typename ...PRE_ARGS,
		 R(*fn)(PRE_ARGS...)	>
struct hedon_caller
<INDEX ,R(PRE_ARGS...),fn, variadic_state<PRE_ARGS...>,variadic_state<> >{
	static R bind (CONST_ARGUMENTS_REFERENCE,PRE_ARGS...pre_args ){
			return fn(pre_args...);
		};
};

template< int INDEX,
		typename R , typename V,
		typename ...PRE_ARGS,typename ...POST_ARGS,
		 R(*fn)(PRE_ARGS..., V, POST_ARGS...)>
struct hedon_caller
<INDEX ,R(PRE_ARGS...,V, POST_ARGS...),fn,
 variadic_state<PRE_ARGS...>,variadic_state<V, POST_ARGS...> >{
 	typedef R(fntype)(PRE_ARGS...,V, POST_ARGS...);
	static R bind (CONST_ARGUMENTS_REFERENCE,PRE_ARGS...pre_args ){
			V value = hedon_getter_wrapper<fntype,fn,V,INDEX>::get(v8args);
			return hedon_caller<INDEX+1,fntype ,fn,variadic_state<PRE_ARGS...,V>, variadic_state<POST_ARGS...> >
			::bind(v8args,pre_args...,value);
		};
};

template< int INDEX,
		typename R , typename V,
		typename ...POST_ARGS,
		 R(*fn)(V, POST_ARGS...)>
struct hedon_caller<INDEX ,R(V, POST_ARGS...),fn>{
	typedef R(fntype)(V, POST_ARGS...);
	static R bind (CONST_ARGUMENTS_REFERENCE ){
			V value = hedon_getter_wrapper<fntype,fn,V,INDEX>::get(v8args);
			return hedon_caller<INDEX+1,fntype ,fn,variadic_state<V>, variadic_state<POST_ARGS...> >
			::bind(v8args,value);
		};
};