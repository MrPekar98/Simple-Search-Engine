#include "shingles.hpp"

namespace crawler
{
    // Prototypes.
    std::set<std::string> set_intersection(const std::set<std::string>& s1, const std::set<std::string>& s2);
    std::set<std::string> set_union(const std::set<std::string>& s1, const std::set<std::string>& s2);

    // Computes Jaccard coefficient.
    double jaccard(const std::set<std::string>& set1, const std::set<std::string>& set2)
    {
        return (double) set_intersection(set1, set2).size() / (double) set_union(set1, set2).size();
    }

    // Returns intersection of two sets.
    std::set<std::string> set_intersection(const std::set<std::string>& s1, const std::set<std::string>& s2)
    {
        std::set<std::string> ret_set;

        for (std::set<std::string>::iterator it1 = s1.begin(); it1 != s1.end(); it1++)
        {
            for (std::set<std::string>::iterator it2 = s2.begin(); it2 != s2.end(); it2++)
            {
                if (it1->compare(*it2))
                    ret_set.insert(*it1);
            }
        }

        return ret_set;
    }

    // Returns union of two sets.
    std::set<std::string> set_union(const std::set<std::string>& s1, const std::set<std::string>& s2)
    {
        std::set<std::string> ret_set = s1;
        ret_set.insert(s2.begin(), s2.end());

        return ret_set;
    }

    // Returns set of shingles of specified length.
    std::set<std::string> shingles(const std::string& str, unsigned short shingle_count)
    {

    }
}
