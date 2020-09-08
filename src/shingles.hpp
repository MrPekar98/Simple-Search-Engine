#include <set>
#include <string>

#define JACC_THRES 0.9
#define SHING_LEN 4

namespace crawler
{
    double jaccard(const std::set<std::string>& set1, const std::set<std::string>& set2);
    std::set<std::string> shingles(const std::string& str, unsigned short shingle_count);
}
