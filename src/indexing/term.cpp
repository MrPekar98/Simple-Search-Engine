#include "term.hpp"
#include <fstream>
#include <string.h>
#include <ctype.h>

#define STOP_WORDS "indexing/stop_word.txt"

namespace scam::indexing
{
    // Term constructor.
    term::term(const std::string& str) noexcept
        : str(str)
    {
        unsigned length = this->str.length();

        for (unsigned i = 0; i < length; i++)
        {
            this->str[i] = tolower(this->str[i]);
        }
    }

    // Getter to string.
    std::string term::get_str() const noexcept
    {
        return this->str;
    }

    // Tokenizes string.
    term& term::tokenize() noexcept
    {
        return *this;
    }

    // Normalizes string.
    term& term::normalize() noexcept
    {
        return *this;
    }

    // Uses Porter's algorithm to stem string.
    term& term::stem() noexcept
    {
        return *this;
    }

    // Check string for being a stop word.
    bool term::is_stop_word() const noexcept
    {
        //char* line = new char[20];
        std::string line;
        std::ifstream stream(STOP_WORDS);

        while (stream >> line)
        {
            if (std::string(line).compare(this->str) == 0)
                return true;
        }

        stream.close();
        return false;
    }
}