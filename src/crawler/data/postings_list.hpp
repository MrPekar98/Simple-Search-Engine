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
        std::vector<document> docs;
        bool building = false;
        void build_postings() noexcept;
        bool word_exists(const std::string& word) const noexcept;
        static std::set<std::string> terms(const std::string& str) noexcept;

    public:
        postings_list(const std::vector<document>& docs) noexcept;
        void reload(const std::vector<document>& docs) noexcept;
        std::vector<std::string> search(const std::string& query) const noexcept;
    };
}