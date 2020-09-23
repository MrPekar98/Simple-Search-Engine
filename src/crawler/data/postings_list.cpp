#include "postings_list.hpp"
#include "pair.hpp"
#include "../../indexing/term.hpp"
#include <stdexcept>
#include <thread>
#include <stdexcept>
#include <utility>
#include <algorithm>

namespace scam::indexing
{
    // Prototypes.
    static inline bool has_document(unsigned id, const std::vector<scam::crawler::document>& docs);
    static std::vector<scam::crawler::document> convert_to_documents(const std::vector<scam::crawler::pair<unsigned, scam::crawler::document>> docs);
    
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

        return rank_documents(query_terms, docs);
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

    // Ranks vector of document by idf scores.
    std::vector<scam::crawler::document> postings_list::rank_documents(const std::set<std::string>& query_terms, const std::vector<scam::crawler::document>& docs) noexcept
    {
        typedef scam::crawler::document doc_t;
        typedef scam::crawler::pair<unsigned, doc_t> ds_pair;
        
        //std::vector<std::pair<unsigned, doc_t>> freq_scores;
        std::vector<ds_pair> freq_scores;
        unsigned doc_length = docs.size();

        for (unsigned i = 0; i < doc_length; i++)
        {
            unsigned doc_idf_score = 0;

            for (const auto& term : query_terms)
            {
                doc_idf_score += term_count(term, docs[i].content);
            }

            //freq_scores.push_back(std::pair<unsigned, doc_t>(doc_idf_score, docs[i]));
            freq_scores.push_back(ds_pair(doc_idf_score, docs[i]));
        }

        std::sort(freq_scores.begin(), freq_scores.end(), [](const ds_pair& d1, const ds_pair& d2){
            return d1.first > d2.first;
        });

        return convert_to_documents(freq_scores);
    }

    // Counts occurences of a term in string of terms.
    unsigned postings_list::term_count(const std::string& term, const std::string& terms_str) noexcept
    {
        std::set<std::string> terms_set = terms(terms_str);
        unsigned count = 0;

        for (const auto& str : terms_set)
        {
            if (str.compare(term) == 0)
                count++;
        }

        return count;
    }

    // Converts vector of pair<unsigned, scam::crawler::document> into vector of scam::crawler::documents.
    static std::vector<scam::crawler::document> convert_to_documents(const std::vector<scam::crawler::pair<unsigned, scam::crawler::document>> docs)
    {
        std::vector<scam::crawler::document> result;
        unsigned length = docs.size();

        for (unsigned i = 0; i < length; i++)
        {
            result.push_back(docs[i].second);
        }

        return result;
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
                scam::indexing::term t = scam::indexing::term(temp).tokenize().stem().normalize();

                if (!t.is_stop_word())
                    terms.insert(t.get_str());

                temp = "";
            }

            else if (i == length - 1)
            {
                temp += str[i];
                scam::indexing::term t = scam::indexing::term(temp).tokenize().stem().normalize();

                if (!t.is_stop_word())
                    terms.insert(scam::indexing::term(temp).tokenize().stem().normalize().get_str());
            }

            else
                temp += str[i];
        }

        return terms;
    }
}