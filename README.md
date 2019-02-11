# HEDON [![hedon tests](https://travis-ci.org/butterfly0x0/HEDON.svg?branch=0.7.0)](https://travis-ci.org/butterfly0x0/HEDON/branches)
HEDON isn't production ready yet.
this project was established to make life easier & simpler for the little worker who need to bind c++ with node.

#### the meaning of HEDON:
- is node.h spelled backwards.
- the mentality behind this project is to achieve a the hedonistic way of code by maximize the pleasure and the simplicity, and to minimize the headaches and the complexity.
- HEDON is built with variadic templates and doesn't inforce you to maintain the interface code, for every change in the core.  

for easy implementation just run in you project directory:
``` bash
$ npm install --save hedonh
```

to add (#include "hedon.h") for easy include in your .cpp files,
all is needed is to add the following lines to binding.gyp : 
``` json
"include_dirs" : [
    "<!(node -e \"require('hedonh')\")"
]
```

hedon should be include in the following hierarchy
``` cpp
#include <node.h>
/*
    ...
*/
#include "hedon.h"
```

for simple include all is needed is just a one line:
``` cpp
double function(float i , double j, int k);

/*
    ...
*/

void Init( v8::Handle<v8::Object> exports) {
    NODE_SET_METHOD(exports, "example", HEDON::BIND(function) );
    /* or  */
    HEDON::EXPORT(exports, function);
  }
NODE_MODULE(hello, Init);

```


(currently in development)
getters add setters are adjustable like so:
``` cpp
struct test{
	int i;
};

test getStruct (){
	test t;
	return t;
}

void Init( v8::Handle<v8::Object> exports) {
	BIND_STRUCT(test
			,BIND_STRUCT_KEY(test,i)
		);
    HEDON::EXPORT(exports, getStruct);
}

NODE_MODULE(hello, Init);
``` 



