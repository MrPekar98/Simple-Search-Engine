#ifndef PAGERANK_HPP
#define PAGERANK_HPP

#include <set>
#include "../crawler/data/document.hpp"

namespace scam
{
	std::set<crawler::document> pagerank(const std::set<crawler::document>& docs) noexcept;
}

#endif
