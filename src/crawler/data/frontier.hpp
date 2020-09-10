#include <queue>
#include <string>
#include <vector>
#include <exception>

namespace scam::crawler
{
    // Simple frontier with no prioritization and politenes..
    class frontier
    {
    protected:
        std::queue<std::string> urls;
        virtual bool url_exists(const std::string& url) noexcept;

    public:
        virtual ~frontier();
        virtual void add_url(const std::string& url) noexcept;
        virtual std::string get_next() noexcept;
        virtual bool empty() const noexcept;
    };

    // Class with front and back queues.
    class mercator : private frontier
    {
    private:
        std::vector<std::queue<std::string>> front_queue;
        std::vector<std::queue<std::string>> back_queue;

    public:
        void add_url(const std::string& url, unsigned short priority) throw();
        bool empty() const noexcept override;
        std::string get_next() noexcept override;
    };

    // Exception class.
    class priority_exception: public std::exception
    {
    public:
        const char* what() const noexcept override;
    };
}
