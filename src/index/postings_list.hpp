#ifndef POSTINGS_LIST_HPP
#define POSTINGS_LIST_HPP

#include "index.hpp"
#include "../document.hpp"
#include <string>
#include <set>

namespace Pekar
{
    class PostingsList: public Index<std::string, std::set<Document>>
    {
    public:
        PostingsList();
    };
}

#endif