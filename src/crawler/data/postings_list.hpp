#include <vector>
#include <map>
#include <string>
#include "document.hpp"

namespace scam::crawler::indexing
{
    class postings_list
    {
    private:
        std::map<std::string, std::vector<unsigned>> postings;
        std::vector<document> docs;
        void build_postings() noexcept;
        bool word_exists(const std::string& word) const noexcept;

    public:
        postings_list(const std::vector<document>& docs) noexcept;
        void reload(const std::vector<document>& docs) noexcept;
        std::vector<std::string> search(const std::string& query) const noexcept;
    };
}