#include <vector>
#include <string>

namespace crawler
{
    // Prototypes.
    std::vector<std::string> crawl(const std::vector<std::string>& urls);
    std::vector<std::string> crawl(const std::vector<std::string>& urls, unsigned long doc_limit);
}
