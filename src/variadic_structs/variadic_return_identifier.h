template<typename T> 
	struct variadic_return_identifier{};

template<typename R > 
	struct variadic_return_identifier<R()>{
		typedef R r;
	};

template<typename R , typename ...TARGS> 
	struct variadic_return_identifier<R(TARGS...)>{
		typedef R r;
	};