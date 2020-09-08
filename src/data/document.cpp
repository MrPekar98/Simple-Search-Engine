#include "document.hpp"

namespace crawler
{
    document::document(const std::string&url, const std::string& content) noexcept
        : url(url), content(content)
    {}
}
