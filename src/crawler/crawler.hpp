#include <set>
#include <vector>
#include <string>
#include "data/document.hpp"

#define USER_AGENT "scam agent"

namespace scam::crawler
{
    // Prototypes.
    void crawl(const std::set<std::string>& urls, std::vector<document>& result_documents);
}
