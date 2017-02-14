template <typename FNTYPE> 
	struct hedon_validator {};

static char missingArgs [] = "missing arguments!";
static char toManyArgs [] = "to many arguments!";

template <typename R,typename ...VARGS> 
	struct hedon_validator<R(VARGS...)>  {
		static char * check(CONST_ARGUMENTS_REFERENCE) {
			int length = variadic_counter<VARGS...>::i;

			if(length > v8args.Length()){
				return missingArgs;
			}

			if(length  < v8args.Length() ){
				return toManyArgs;
			}

			return NULL;
		};
	};
