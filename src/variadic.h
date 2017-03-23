
/*
	state management
*/
template< typename ... ARGS> 
	struct state {};

/*
	counter
*/ 
template< typename ... ARGS> 
	struct counter {};
template<>
	struct counter<> {
	    static const int n = 0;
	};
template<typename T, typename... VARGS>
	struct counter<T, VARGS...> {
	    static const int n = 1 + counter<VARGS...>::n;
	};
/*
	index sequence
*/
template <int...> struct sequence {};

template <class S1, class S2> struct concat_sequence;
template <class S1, class S2> using  concat_sequence_t = typename concat_sequence<S1, S2>::i;

template <int... I1, int... I2>
	struct concat_sequence<sequence<I1...>, sequence<I2...> > {
	  using i = sequence<I1..., (sizeof...(I1)+I2)...>;
	};

/*
	make index sequence
*/
template <int N> 
	struct make_sequence;
template <int N> 
	using  make_sequence_t = typename make_sequence<N>::i;

template <int N>
	struct make_sequence {
	  using i = concat_sequence_t<make_sequence_t<N/2>, make_sequence_t<N - N/2>>;
	};

template <> 
	struct make_sequence<1> { 
		using i = sequence<0>; 
	};
template <> 
	struct make_sequence<0> { 
		using i = sequence<>;
		 };

/*
	sizeof index sequence
*/
template <int... Is>
	constexpr size_t size(sequence<Is...>) {
	  return sizeof...(Is);
	}

/*
	indexes 
*/
template <typename ... ARGS > 
	using  indexes = typename make_sequence< counter<ARGS...>::n >::i;

