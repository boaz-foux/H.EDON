# HEDON
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
#include <v8.h>
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
  }
NODE_MODULE(hello, Init);

```


getters add setters are adjustable like so:
``` cpp
CLASS_TYPE get(const v8::Local<v8::Value> & input){ /* ... */ }
v8::Local<v8::Value> set(v8::Isolate * isolate ,CLASS_TYPE _class) { /* ... */ }
/*
    ...
*/
#include "hedon.h"
HEDON::GETTER(CLASS_TYPE ,get);
HEDON::SETTER(CLASS_TYPE ,set);
``` 

HEDON has been tasted on OS X  ( el capitan )
