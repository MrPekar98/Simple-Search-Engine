#ifndef QUERY_HPP
#define QUERY_HPP

#include <vector>
#include "term.hpp"

namespace Pekar
{
    class Query
    {
    public:
        Query(const std::vector<Term>& terms);
        Query(const Query& other) = default;
        Query(Query&& other) = default;
        std::string queryString() const noexcept;
        const Term& operator[](const unsigned& i) const;
        static Query make(const std::string& queryString) noexcept;

    private:
        std::vector<Term> query;
    };
}

#endif