#include <queue>
#include <string>

namespace crawler
{
    // Simple frontier with no prioritization and politenes..
    class frontier
    {
    private:
        std::queue<std::string> urls;
        bool url_exists(const std::string& url) noexcept;

    public:
        void add_url(const std::string& url) noexcept;
        std::string get_next() noexcept;
        bool empty() const noexcept;
    };
}
