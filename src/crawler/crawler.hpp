#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <set>
#include <string>
#include "../index/postings_list.hpp"

#define USER_AGENT "pekar"

namespace Pekar
{
    void crawl(const std::set<std::string>& seedSet, PostingsList& pl, const unsigned& threadCount) noexcept;
}

#endif