#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <vector>
#include <string>
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
                if (*it == ' ')
                {
                    terms.push_back(inter);
                    inter = "";
                    continue;
                }

                inter += *it;
            }

            terms.push_back(inter);
            return terms;
        }
    };
}

#endif