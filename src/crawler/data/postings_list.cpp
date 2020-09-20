#include "postings_list.hpp"
#include "../../indexing/term.hpp"
#include <stdexcept>
#include <thread>
#include <stdexcept>

namespace scam::indexing
{
    // Prototypes.
    static inline bool has_document(unsigned id, const std::vector<scam::crawler::document>& docs);

    // Constructor.
    postings_list::postings_list(const std::vector<scam::crawler::document>& docs) noexcept
        : docs(docs)
    {
        build_postings();
    }

    // Builds inverted index.
    void postings_list::build_postings(unsigned from_docs_index) noexcept
    {
        unsigned length = this->docs.size();

        for (unsigned i = from_docs_index; i < length; i++)
        {
            std::set<std::string> ts = postings_list::terms(this->docs[i].content);

            for (std::set<std::string>::iterator it = ts.begin(); it != ts.end(); it++)
            {
                scam::indexing::term t = scam::indexing::term(*it);

                if (t.is_stop_word())
                    continue;

                std::string term_str = t.get_str();
                
                if (!word_exists(term_str))
                    this->postings[term_str] = std::set<unsigned>();
                
                this->postings[term_str].insert(this->docs[i].id);
            }
        }
    }

    // Updates inverted index.
    void postings_list::reload(const std::vector<scam::crawler::document>& docs) noexcept
    {
        unsigned length = docs.size();
        unsigned build_start = this->docs.size();

        for (unsigned i = 0; i < length; i++)
        {
            if (!has_document(docs[i].id, this->docs))
                this->docs.push_back(docs[i]);
        }

        build_postings(build_start);
    }

    // Searches inverted index by given query.
    std::vector<scam::crawler::document> postings_list::search(const std::string& query) const noexcept
    {
        std::set<std::string> query_terms = terms(query);
        std::vector<scam::crawler::document> docs;

        for (std::set<std::string>::iterator query_it = query_terms.begin(); query_it != query_terms.end(); query_it++)
        {
            try
            {
                std::set<unsigned> doc_ids = this->postings.at(*query_it);
                unsigned length = doc_ids.size();

                for (std::set<unsigned>::iterator id_it = doc_ids.begin(); id_it != doc_ids.end(); id_it++)
                {
                    scam::crawler::document doc = find_document(*id_it);

                    if (!has_document(*id_it, docs))
                        docs.push_back(doc);
                }
            }

            catch (const std::out_of_range& exc)
            {
                // Just continue;
            }
        }

        return docs;
    }

    // Checks vector of documents for document existence.
    static inline bool has_document(unsigned id, const std::vector<scam::crawler::document>& docs)
    {
        unsigned length = docs.size();

        for (unsigned i = 0; i < length; i++)
        {
            if (id == docs[i].id)
                return true;
        }

        return false;
    }

    // Finds document with given ID.
    scam::crawler::document postings_list::find_document(unsigned id) const throw()
    {
        unsigned length = this->docs.size();

        for (unsigned i = 0; i < length; i++)
        {
            if (id == this->docs[i].id)
                return this->docs[i];
        }

        throw std::out_of_range("ID mismatch");
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

    // Returns set of terms.
    // Normalization, and stemming.
    // Stop word are not removed here.
    std::set<std::string> postings_list::terms(const std::string& str) noexcept
    {
        std::set<std::string> terms;
        std::string temp = "";
        unsigned length = str.length();

        for (unsigned i = 0; i < length; i++)
        {
            if (str[i] == ' ')
            {
                terms.insert(scam::indexing::term(temp).tokenize().stem().normalize().get_str());
                temp = "";
            }

            else if (i == length - 1)
            {
                temp += str[i];
                terms.insert(scam::indexing::term(temp).tokenize().stem().normalize().get_str());
            }

            else
                temp += str[i];
        }

        return terms;
    }
}