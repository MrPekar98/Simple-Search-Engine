#include "document.hpp"
#include <hash.h>

namespace scam::crawler
{
    document::document(const std::string&url, const std::string& content, std::set<std::string> out_links) noexcept
        : url(url), content(content)
    {
        this->id = int16_checksum(this->content.c_str(), this->content.length());
        this->out_links = out_links;
    }

    document document::operator=(const document& doc) noexcept
    {
        this->id = doc.id;
        this->url = doc.url;
        this->content = doc.content;
        return *this;
    }
}
