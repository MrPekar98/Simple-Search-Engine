#ifndef SHINGLE_HPP
#define SHINKGLE_HPP

#include <string>
#include <set>

namespace Pekar
{
    class Shingle
    {
    public:
        static Shingle buildShingles(const std::string& str, const unsigned& shingleLength);
        Shingle(const Shingle& other) = default;
        Shingle(Shingle&& other) = default;
        Shingle() = delete;
        const std::string& getBaseStr() const noexcept;
        const std::set<std::string>& getShingles() const noexcept;

    private:
        Shingle(const std::string& str);
        std::set<std::string> shingles;
        std::string base;
    };
}

#endif