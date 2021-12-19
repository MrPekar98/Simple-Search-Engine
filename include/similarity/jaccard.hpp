#ifndef JACCARD_HPP
#define JACCARD_HPP

#include <similarity/shingle.hpp>
#include <algorithm>

namespace Pekar
{
    class Jaccard
    {
    public:
        template<typename E>
        static double jaccard(const std::set<E>& s1, const std::set<E>& s2) noexcept
        {
            std::set<E> merged(s1);
            merged.insert(s2.cbegin(), s2.cend());
            return (double) Jaccard::intersectionCount(s1, s2) / merged.size();
        }

    private:
        template<typename E1, typename E2>
        static unsigned intersectionCount(const std::set<E1>& s1, const std::set<E2>& s2) noexcept
        {
            unsigned count = 0;
            std::for_each(s1.cbegin(), s1.cend(), [&s2, &count](const auto& e){ if (std::find(s2.cbegin(), s2.cend(), e) != s2.cend()) count++; });
            return count;
        }
    };
}

#endif