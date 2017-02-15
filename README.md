# HEDON.H
this project was established to make life easier & simpler for the little worker who need to bind c++ with node.
for easy implamentation just run in you project direcotry:
``` bash
$ npm install --save hedonh
```
to add (#include "hedon.h") for easy include in your .cpp files,
all is needed is to add the following lines to binding.gyp : 
``` python
"include_dirs" : [
    "<!(node -e \"require('hedonh')\")"
]
```

hedon should be include in the following hierarchy
``` 
#include <v8.h>
#include <node.h>
/*
    ...
*/
#include "hedon.h"
```

for simple include all is needed is just a one line:
``` 
NODE_SET_METHOD(exports, "hello", HEDON::BIND(function) );
```
