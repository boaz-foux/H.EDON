#ifndef ___PURE__HEDON__BIND___H___
#define ___PURE__HEDON__BIND___H___
namespace HEDON {

#define  ARGUMENTS_TYPE \
 	const v8::FunctionCallbackInfo<v8::Value> &

#define CONST_ARGUMENTS_REFERENCE \
	ARGUMENTS_TYPE v8args

#include "./variadic_structs/variadic_state.h"
#include "./variadic_structs/variadic_counter.h"
#include "./variadic_structs/variadic_return_identifier.h"
#include "./v8_structs/validator.h"
#include "./v8_structs/setter.h"
#include "./v8_structs/getter.h"
#include "./v8_structs/memory.h"
#include "./v8_structs/getter_wrapper.h"
#include "./v8_structs/caller.h"
#include "./v8_structs/binder.h"

template < typename FNTYPE,FNTYPE * fn ,int SIZE>
	void Bind(CONST_ARGUMENTS_REFERENCE){ 
		 hedon_binder< typename variadic_return_identifier<FNTYPE>::r,FNTYPE,fn,SIZE>::bind(v8args);
	};

#define BIND(FUNCTION) \
	Bind<decltype(FUNCTION), FUNCTION,0>

#define BIND_ARRAY(FUNCTION,N) \
	Bind<decltype(FUNCTION), FUNCTION,N>	

}
#endif /* ___PURE__HEDON__BIND___H___ */
