#include "postings_list.hpp"
#include <stdexcept>

namespace scam::crawler::indexing
{
    // Constructor.
    postings_list::postings_list(const std::vector<document>& docs) noexcept
        : docs(docs)
    {

    }

    // Builds inverted index.
    void postings_list::build_postings() noexcept
    {

    }

    // Updates inverted index.
    void postings_list::reload(const std::vector<document>& docs) noexcept
    {

    }

    // Searches inverted index by given query. Returns ranked list of URLS.
    std::vector<std::string> postings_list::search(const std::string& query) const noexcept
    {

    }

    // Checks for existence of word in inverted index.
    bool postings_list::word_exists(const std::string& word) const noexcept
    {
        try
        {
            this->postings.at(word);
            return true;
        }

        catch (const std::out_of_range& exc)
        {
            return false;
        }
    }
}