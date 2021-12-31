#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <vector>
#include <string>
#include <cctype>
#include "query/term.hpp"

namespace Pekar
{
    class Tokenizer
    {
    public:
        static std::vector<Term> tokenize(const std::string& str) noexcept
        {
            std::vector<Term> terms;
            std::string inter = "";

            for (auto it = str.cbegin(); it != str.cend(); it++)
            {
                if (Tokenizer::isSpecialCharacter(*it))
                {
                    if (inter.size() > 0 && !Tokenizer::containsSpecialCharacters(inter))
                        terms.push_back(Tokenizer::manipulate(inter));
                    
                    inter = "";
                    continue;
                }

                inter += *it;
            }

            terms.push_back(Tokenizer::manipulate(inter));
            return terms;
        }

    private:
        static bool isSpecialCharacter(const char& c) noexcept
        {
            return c < 65 || (c > 90 && c < 97) || c > 122;
        }

        static bool containsSpecialCharacters(const std::string& str) noexcept
        {
            for (const char& c : str)
            {
                if (Tokenizer::isSpecialCharacter(c))
                    return true;
            }

            return false;
        }

        static std::string manipulate(const std::string& str) noexcept
        {
            std::string manipulated = "";

            for (const char& c : str)
            {
                manipulated += std::tolower(c);
            }

            return manipulated;
        }
    };
}

#endif