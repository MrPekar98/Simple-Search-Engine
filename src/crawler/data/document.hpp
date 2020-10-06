#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include <string>
#include <set>

namespace scam::crawler
{
    struct document
    {
        std::string url, content;
        unsigned id;
        std::set<std::string> out_links;

        document(const std::string& url, const std::string& content, std::set<std::string> out_links = std::set<std::string>()) noexcept;
        document operator=(const document& doc) noexcept;
    };
}

#endif
