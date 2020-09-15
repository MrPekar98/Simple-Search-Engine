#include <string>

namespace scam::crawler
{
    struct document
    {
        const std::string url, content;
        unsigned id;

        document(const std::string& url, const std::string& content) noexcept;
    };
}
