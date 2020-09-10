#include <queue>
#include <string>

namespace scam::crawler
{
    // Simple frontier with no prioritization and politenes..
    class frontier
    {
    private:
        std::queue<std::string> urls;

    protected:
        bool url_exists(const std::string& url) noexcept;

    public:
        virtual ~frontier();
        virtual void add_url(const std::string& url) noexcept;
        virtual std::string get_next() noexcept;
        virtual bool empty() const noexcept;
    };
}
