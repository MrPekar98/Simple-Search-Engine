#include "shingles.hpp"
#include <vector>

namespace scam::crawler
{
    // Prototypes.
    std::set<std::string> set_intersection(const std::set<std::string>& s1, const std::set<std::string>& s2);
    std::set<std::string> set_union(const std::set<std::string>& s1, const std::set<std::string>& s2);
    static std::vector<std::string> str_terms(const std::string& str);

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
        std::set<std::string> shingles;
        std::vector<std::string> terms = str_terms(str);

        for (unsigned i = 0; i < terms.size(); i++)
        {
            std::string shingle = "";

            for (unsigned j = 0; j < shingle_count; j++)
            {
                if (i + j == terms.size())
                    break;

                shingle += terms[i + j];
                shingle += ' ';
            }

            shingle.erase(shingle.length() - 1);
            shingles.insert(shingle);
        }

        return shingles;
    }

    // Creates set of terms in string.
    static std::vector<std::string> str_terms(const std::string& str)
    {
        std::vector<std::string> terms;
        std::string temp = "";
        const char* c_str = str.c_str();
        unsigned length = str.length();

        for (unsigned i = 0; i < length; i++)
        {
            if (c_str[i] == ' ')
            {
                terms.push_back(temp);
                temp = "";
            }

            else if (i == length - 1)
            {
                temp += c_str[i];
                terms.push_back(temp);
            }

            else
                temp += c_str[i];
        }

        return terms;
    }
}
