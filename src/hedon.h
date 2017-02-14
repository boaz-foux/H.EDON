#ifndef ___PURE__HEDON__BIND___H___
#define ___PURE__HEDON__BIND___H___
namespace HEDON {

#include "./macros.h"
#include "./variadic_structs/index.h"
#include "./v8_structs/index.h"

template < typename FNTYPE,FNTYPE * fn >
	void Bind(CONST_ARGUMENTS_REFERENCE){ 
		 hedon_binder< typename variadic_return_identifier<FNTYPE>::r,FNTYPE,fn>::bind(v8args);
	};

#define BIND(FUNCTION) \
	Bind<decltype(FUNCTION), FUNCTION>
	

}
#endif /* ___PURE__HEDON__BIND___H___ */
