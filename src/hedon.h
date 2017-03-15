#ifndef ___PURE__HEDON__BIND___H___
#define ___PURE__HEDON__BIND___H___
namespace HEDON {

#include "./variadic.h"
#include "./v8.h"
typedef void (* BINDING_FN_TYPE)(const v8::FunctionCallbackInfo<v8::Value> &);

void temp (const v8::FunctionCallbackInfo<v8::Value> &) {};

template <typename UNKNOWN>
 BINDING_FN_TYPE Bind( UNKNOWN u){
 return temp;
};




#define BIND(FUNCTION) \
	Bind(FUNCTION)

#define BIND_ARRAY(FUNCTION,N) \
	Bind(FUNCTION)

}
#endif /* ___PURE__HEDON__BIND___H___ */
