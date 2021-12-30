#include "document_search.hpp"
#include "ranker.hpp"
#include <algorithm>

namespace Pekar
{
    std::vector<Document> DocumentSearch::search(const Query& q, PostingsList& invertedIndex, const double& similarityThreshold) noexcept
    {
        std::vector<Document> queryDocuments = DocumentSearch::documents(q, invertedIndex);
        return Ranker::rank(q, queryDocuments, similarityThreshold);
    }

    std::vector<Term> DocumentSearch::terms(const Query& q) noexcept
    {
        std::vector<Term> tv;
        unsigned len = q.length();

        for (unsigned i = 0; i < len; i++)
        {
            tv.push_back(q[i]);
        }

        return tv;
    }

    std::vector<Document> DocumentSearch::documents(const Query& q, PostingsList& pl) noexcept
    {
        std::vector<Term> tv = DocumentSearch::terms(q);
        std::vector<Document> docs;

        for (const Term& t : tv)
        {
            std::set<Document> foundDocs = pl.find(t.getTerm());
            
            for (const Document& doc : foundDocs)
            {
                if (std::find(docs.cbegin(), docs.cend(), doc) == std::end(docs))
                    docs.push_back(doc);
            }
        }

        return docs;
    }
}