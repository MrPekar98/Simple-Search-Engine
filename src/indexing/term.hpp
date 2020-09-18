#ifndef TERM_HPP
#define TERM_HPP

#include <string>

namespace scam::indexing
{
    // TODO: Add function to tokenize, stem, and so on...
    class term
    {
    private:
        std::string str;

    public:
        term(const std::string& str) noexcept;
        std::string get_str() const noexcept;
        term& tokenize() noexcept;
        term& normalize() noexcept;
        term& stem() noexcept;
        bool is_stop_word() const noexcept;
    };
}

#endif