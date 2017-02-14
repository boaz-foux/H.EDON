
template<typename T> 
	struct hedon_getter {};

template<typename TYPE> 
	struct hedon_getter<TYPE *>{
		static TYPE * get(const v8::Local<v8::Value> &i){
			int64_t m = i->NumberValue();
			TYPE * p  = (TYPE *)m;
			return p;
		};
	};

template<> 
	struct hedon_getter<bool>{
		static bool  get(const v8::Local<v8::Value> &i){
			return i->BooleanValue();
		};
	};

template<> 
	struct hedon_getter<float>{
		static float get(const v8::Local<v8::Value> &i){
			return i->NumberValue();
		};
	};

template<> 
	struct hedon_getter<double>{
		static double get(const v8::Local<v8::Value> &i){
			return i->NumberValue();
		};
	};

template<> 
	struct hedon_getter<int32_t>{
		static  int32_t get(const v8::Local< v8::Value> &i){
			return i->Int32Value();
		};
	};

template<> 
	struct hedon_getter<int64_t>{
		static int32_t get(const v8::Local<v8::Value> &i){
			return i->IntegerValue();
		};
	};

template<> 
	struct hedon_getter<uint32_t>{
		static  uint32_t get(const v8::Local<v8::Value> &i){
			return i->Uint32Value();
		};
	};

#define GETTER(TYPE,FUNCTION) \
	template<> \
		struct hedon_getter<TYPE>{ \
			static TYPE get(const v8::Local<v8::Value> &i){ \
				return FUNCTION(i); \
			} \
		}