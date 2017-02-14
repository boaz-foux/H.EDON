
template<typename... VARGS> struct variadic_counter;

template<>
struct variadic_counter<> {
    static const int i = 0;
};

template<typename T, typename... VARGS>
struct variadic_counter<T, VARGS...> {
    static const int i = 1 + variadic_counter<VARGS...>::i;
};