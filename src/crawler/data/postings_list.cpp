#include "postings_list.hpp"
#include <stdexcept>
#include <thread>

namespace scam::indexing
{
    // Constructor.
    postings_list::postings_list(const std::vector<scam::crawler::document>& docs) noexcept
        : docs(docs)
    {
        build_postings();
    }

    // Builds inverted index.
    void postings_list::build_postings() noexcept
    {
        unsigned length = this->docs.size();

        for (unsigned i = 0; i < length; i++)
        {
            std::set<std::string> ts = postings_list::terms(this->docs[i].content);

            for (std::set<std::string>::iterator it = ts.begin(); it != ts.end(); it++)
            {
                if (!word_exists(*it))
                    this->postings[*it] = std::vector<unsigned>();
                
                this->postings[*it].push_back(this->docs[i].id);
            }
        }
    }

    // Updates inverted index.
    void postings_list::reload(const std::vector<scam::crawler::document>& docs) noexcept
    {
        this->docs.clear();
        unsigned length = docs.size();

        for (unsigned i = 0; i < length; i++)
        {
            this->docs.push_back(docs[i]);
        }

        build_postings();
    }

    // Searches inverted index by given query. Returns ranked list of URLS.
    /*std::vector<std::string> postings_list::search(const std::string& query) const noexcept
    {

    }*/

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

    // TODO: Add functions to manipulate each term.
    // Returns set of terms.
    // Stop words are removed, normalization, and stemming.
    std::set<std::string> postings_list::terms(const std::string& str) noexcept
    {
        std::set<std::string> terms;
        std::string temp = "";
        unsigned length = str.length();

        for (unsigned i = 0; i < length; i++)
        {
            if (str[i] == ' ')
            {
                terms.insert(temp);
                temp = "";
            }

            else if (i == length - 1)
            {
                temp += str[i];
                terms.insert(temp);
            }

            else
                temp += str[i];
        }

        return terms;
    }
}