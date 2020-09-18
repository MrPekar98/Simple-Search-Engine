#include "term.hpp"
#include <fstream>
#include <string.h>
#include <stem.h>

#define STOP_WORDS "indexing/stop_word.txt"

namespace scam::indexing
{
    // Term constructor.
    term::term(const std::string& str) noexcept
    {
        //std::transform(str.begin(), str.end(), str.begin(), tolower);
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
        char* char_str = new char[this->str.length()];
        memcpy(char_str, this->str.c_str(), this->str.length());

        unsigned end_index = ::stem(char_str, 0, this->str.length());
        char_str[end_index] = '\0';
        this->str = char_str;

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