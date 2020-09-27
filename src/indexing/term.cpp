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
        delete_trailing_special_chars();
        delete_leading_special_chars();

        return *this;
    }

    // Deletes trailing special characters.
    void term::delete_trailing_special_chars() noexcept
    {
        unsigned length = this->str.size();
        
        for (unsigned i = length - 1; i >= 0; i--)
        {
            if (((this->str[i] >= 48 && this->str[i] <= 57) || 
                    (this->str[i] >= 65 && this->str[i] <= 90) || 
                    (this->str[i] >= 97 && this->str[i] <= 122)) && i != length - 1)
            {
                this->str.erase(i + 1, length - i);
                break;
            }

            else if ((this->str[i] >= 48 && this->str[i] <= 57) || 
                        (this->str[i] >= 65 && this->str[i] <= 90) || 
                        (this->str[i] >= 97 && this->str[i] <= 122))
                break;
        }
    }

    // Deletes leading special characters.
    void term::delete_leading_special_chars() noexcept
    {
        unsigned length = this->str.size();

        for (unsigned i = 0; i < length; i++)
        {
            if (((this->str[i] >= 48 && this->str[i] <= 57) || 
                    (this->str[i] >= 65 && this->str[i] <= 90) || 
                    (this->str[i] >= 97 && this->str[i] <= 122)) && i != 0)
            {
                this->str.erase(0, i);
                break;
            }

            else if ((this->str[i] >= 48 && this->str[i] <= 57) || 
                        (this->str[i] >= 65 && this->str[i] <= 90) || 
                        (this->str[i] >= 97 && this->str[i] <= 122))
                break;
        }
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