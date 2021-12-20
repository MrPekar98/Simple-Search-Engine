#ifndef TERM_HPP
#define TERM_HPP

#include <string>
#include <utility>
#include <fstream>
#include <set>
#include "../config.hpp"

namespace Pekar
{
    class Term
    {
    public:
        template <typename Str>
        Term(Str&& lexeme)
            : term(std::forward<Str>(lexeme))
        {
            normalize();
            stem();
        }

        const std::string& getTerm() const noexcept
        {
            return this->term;
        }

        // It is more efficient to open file when this method is invoked compared
        // to loading all stop words in the constructor
        bool isStopWord() noexcept
        {
            if (this->stopWords.size() > 0)
                return this->stopWords.find(this->term) != this->stopWords.end();
            
            std::fstream stream(STOP_WORDS, std::ios_base::in);

            if (!stream)
                return false;

            while (!stream.eof())
            {
                char* buffer = new char[30];
                stream.getline(buffer, 30);

                std::string str(buffer);
                str.erase(str.size() - 1);
                this->stopWords.insert(std::move(str));
                delete[] buffer;
            }

            return this->stopWords.find(this->term) != this->stopWords.end();
        }

    private:
        void stem() noexcept
        {

        }

        void normalize() noexcept
        {

        }

        std::string term;
        std::set<std::string> stopWords;
    };
}

#endif