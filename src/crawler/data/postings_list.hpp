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
        std::map<std::string, std::vector<unsigned>> postings;
        std::vector<scam::crawler::document> docs;
        bool building = false;
        void build_postings() noexcept;
        bool word_exists(const std::string& word) const noexcept;
        static std::set<std::string> terms(const std::string& str) noexcept;
        scam::crawler::document find_document(unsigned id) const throw();

    public:
        postings_list(const std::vector<scam::crawler::document>& docs) noexcept;
        void reload(const std::vector<scam::crawler::document>& docs) noexcept;
        std::vector<scam::crawler::document> search(const std::string& query) const noexcept;
    };
}