#include <set>
#include <string>

#define JACC_THRES 0.9

namespace crawler
{
    double jaccard(const std::set<std::string>& set1, const std::set<std::string>& set2);
}
