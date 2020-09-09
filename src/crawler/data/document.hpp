#include <string>

namespace scam::crawler
{
    struct document
    {
        const std::string url, content;

        document(const std::string& url, const std::string& content) noexcept;
    };
}
