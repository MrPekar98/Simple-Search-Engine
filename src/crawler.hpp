#include <vector>
#include <string>
#include "data/document.hpp"

#define USER_AGENT "scam agent"

namespace scam::crawler
{
    // Prototypes.
    std::vector<document> crawl(const std::vector<std::string>& urls);
    std::vector<document> crawl(const std::vector<std::string>& urls, unsigned long doc_limit);
}
