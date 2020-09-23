#include "document.hpp"
#include <hash.h>

namespace scam::crawler
{
    document::document(const std::string&url, const std::string& content) noexcept
        : url(url), content(content)
    {
        this->id = int16_checksum(this->content.c_str(), this->content.length());
    }

    document document::operator=(const document& doc) noexcept
    {
        this->id = doc.id;
        this->url = doc.url;
        this->content = doc.content;
        return *this;
    }
}
