#ifndef DOCUMENT_SEARCH_HPP
#define DOCUMENT_SEARCH_HPP

#include "query.hpp"
#include "../index/postings_list.hpp"
#include "../config.hpp"
#include <vector>
#include <string>

namespace Pekar
{
    class DocumentSearch
    {
    public:
        static std::vector<Document> search(const Query& q, PostingsList& invertedIndex, const double& similarityThreshold) noexcept;

    private:
        static std::vector<Term> terms(const Query& q) noexcept;
        static std::vector<Document> documents(const Query& q, PostingsList& pl) noexcept;
    };
}

#endif