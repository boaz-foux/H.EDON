# HEDON.H

``` bash
$ npm install --save hedonh
```

``` python
"include_dirs" : [
    "<!(node -e \"require('hedonh')\")"
]
```


``` 
#include <node.h>
#include <v8.h>
#include "hedon.h"
```

``` 
NODE_SET_METHOD(exports, "hello", HEDON::BIND(function) );
```