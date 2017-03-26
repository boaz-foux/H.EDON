#ifndef ___PURE__HEDON__BIND___H___
#define ___PURE__HEDON__BIND___H___
#include <string>
#include <typeinfo>
namespace HEDON {

#include "./variadic.h"

#define ISOLATE v8::Isolate::GetCurrent()
#define V8ARGS v8::FunctionCallbackInfo<v8::Value>
typedef 
	void (* BINDING_FN_TYPE)(const v8::FunctionCallbackInfo<v8::Value> &);

#include "./v8.h"


#define BIND(I) binder<decltype(I),I>::bind

// #define BIND_ARRAY(I,N) BIND(I)

}
#endif /* ___PURE__HEDON__BIND___H___ */
