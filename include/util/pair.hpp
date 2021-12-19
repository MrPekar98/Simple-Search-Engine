#ifndef PAIR_HPP
#define PAIR_HPP

#include <utility>

template <typename T1, typename T2>
struct Pair
{
    T1 first;
    T2 second;

    constexpr Pair(T1&& first, T2&& second)
        : first(std::forward<T1>(first)), second(std::forward<T2>(second))
    {}
};

#endif