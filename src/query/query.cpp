#include "query.hpp"
#include "../tokenizer.hpp"

namespace Pekar
{
    Query::Query(const std::vector<Term>& terms)
        : query(terms)
    {}

    std::string Query::queryString() const noexcept
    {
        std::string qStr = "";

        for (auto it = this->query.cbegin(); it != this->query.cend(); it++)
        {
            qStr += it->getTerm() + " ";
        }

        return qStr.erase(qStr.size() - 1);
    }

    const Term& Query::operator[](const unsigned& i) const
    {
        return this->query[i];
    }

    Query Query::make(const std::string& queryString) noexcept
    {
        return Query(Tokenizer::tokenize(queryString));
    }
}