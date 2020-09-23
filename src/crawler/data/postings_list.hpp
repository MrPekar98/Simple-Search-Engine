#include <vector>
#include <map>
#include <string>
#include <set>
#include "document.hpp"

namespace scam::indexing
{
    class postings_list
    {
    private:
        std::map<std::string, std::set<unsigned>> postings;
        std::vector<scam::crawler::document> docs;
        bool building = false;
        void build_postings(unsigned from_docs_index = 0) noexcept;
        bool word_exists(const std::string& word) const noexcept;
        static std::set<std::string> terms(const std::string& str) noexcept;
        scam::crawler::document find_document(unsigned id) const throw();
        static std::vector<scam::crawler::document> rank_documents(const std::set<std::string>& query_terms, const std::vector<scam::crawler::document>& docs) noexcept;
        static unsigned term_count(const std::string& term, const std::string& terms_str) noexcept;

    public:
        postings_list(const std::vector<scam::crawler::document>& docs) noexcept;
        void reload(const std::vector<scam::crawler::document>& docs) noexcept;
        std::vector<scam::crawler::document> search(const std::string& query) const noexcept;
    };
}