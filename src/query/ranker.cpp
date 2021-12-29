#include "ranker.hpp"
#include "../tokenizer.hpp"
#include <map>
#include <algorithm>
#include <stdexcept>
#include <util/pair.hpp>
#include <cmath>

// This implementation is inefficient
// There is a lot of re-iteration
// This is problematic for ranking/scoring large documents

namespace Pekar
{
    std::vector<Document> Ranker::rank(const Query& q, const std::vector<Document>& documents, const double& scoreThreshold)
    {
        if (scoreThreshold < 0 || scoreThreshold > 1)
            throw std::invalid_argument("Score threshold must be between 0 and 1");

        std::vector<Pair<Document, double>> scores;
        std::vector<Document> ranked;

        for (auto it = documents.cbegin(); it != documents.cend(); it++)
        {
            double s = score(q, *it, documents);

            if (s >= scoreThreshold)
                scores.push_back(Pair<Document, double>(Document(*it), std::move(s)));
        }

        std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b){ return a.second >= b.second; });
        std::for_each(scores.cbegin(), scores.cend(), [&ranked](const auto& pair){ ranked.push_back(pair.first); });

        return ranked;
    }

    double Ranker::score(const Query& q, const Document& d, const std::vector<Document>& documents) noexcept
    {
        unsigned len = q.length();
        double s = 0;
        
        for (unsigned i = 0; i < len; i++)
        {
            Term t = q[i];

            if (!t.isStopWord())
                s += tf(t, d) * idf(q[i], documents);
        }

        return s;
    }

    // Frequency in document content
    // Unfortunately, the current tokenizer cannot handle (eliminate) special characters
    // Maybe the vector of terms should be converted to a set so we don't have duplicates
    double Ranker::tf(const Term& t, const Document& d) noexcept
    {
        std::vector<Term> documentTerms = Tokenizer::tokenize(d.getContent());
        unsigned sum = 0;

        for (const Term& dTerm : documentTerms)
        {
            sum += subStringCount(dTerm.getTerm(), d.getContent());
        }

        return (double) subStringCount(t.getTerm(), d.getContent()) / sum;
    }

    unsigned Ranker::subStringCount(const std::string& subStr, const std::string& str) noexcept
    {
        unsigned count = 0;
        size_t pos = -1;

        while ((pos = str.find(subStr, pos + 1)) != std::string::npos)
        {
            count++;
        }

        return count;
    }

    double Ranker::idf(const Term& t, const std::vector<Document>& documents) noexcept
    {
        unsigned termCount = 0;

        for (const Document& d : documents)
        {
            if (d.getContent().find(t.getTerm()) != std::string::npos)
                termCount++;
        }

        return (double) documents.size() / (termCount > 0) ? termCount : 1;
    }
}