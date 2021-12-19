#include <similarity/shingle.hpp>
#include <vector>

namespace Pekar
{
    // Prototype
    static std::vector<std::string> strTerms(const std::string& str);

    // Class builder
    Shingle Shingle::buildShingles(const std::string& str, const unsigned& shingleLength)
    {
        Shingle s(str);
        std::vector<std::string> terms = strTerms(str);

        for (unsigned i = 0; i < terms.size(); i++)
        {
            std::string shingle = "";

            for (unsigned j = i; j < terms.size() && j < shingleLength; j++)
            {
                shingle += terms[j] + ' ';
            }

            if (shingle.size() > 0)
            {
                shingle.erase(shingle.length() - 1);
                s.shingles.insert(shingle);
            }
        }

        return s;
    }

    // Constructer (private)
    Shingle::Shingle(const std::string& str)
        : base(str)
    {}

    // Getter to string base.
    const std::string& Shingle::getBaseStr() const noexcept
    {
        return this->base;
    }

    // Getter to collection of shingles
    const std::set<std::string>& Shingle::getShingles() const noexcept
    {
        return this->shingles;
    }

    static std::vector<std::string> strTerms(const std::string& str)
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