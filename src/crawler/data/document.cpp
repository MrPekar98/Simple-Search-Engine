#include "document.hpp"

namespace scam::crawler
{
    document::document(const std::string&url, const std::string& content) noexcept
        : url(url), content(content)
    {}
}
