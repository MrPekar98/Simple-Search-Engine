#ifndef RANKER_HPP
#define RANKER_HPP

#include <vector>
#include "../document.hpp"
#include "query.hpp"

// Ranks iterator of documents given a query using TF-IDF similarity
namespace Pekar
{
    class Ranker
    {
    public:
        static std::vector<Document> rank(const Query& q, const std::vector<Document>& documents, const double& scoreThreshold);

    private:
        static double score(const Query& q, const Document& d, const std::vector<Document>& documents) noexcept;
        static double tf(const Term& t, const Document& d) noexcept;
        static unsigned subStringCount(const std::string& subStr, const std::string& str) noexcept;
        static double idf(const Term& t, const std::vector<Document>& documents) noexcept;
    };
}

#endif