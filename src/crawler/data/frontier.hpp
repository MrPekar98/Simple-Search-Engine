#include <queue>
#include <string>
#include <vector>
#include <exception>
#include <mutex>
#include <initializer_list>

namespace scam::crawler
{
    // Simple frontier with no prioritization and politenes..
    class frontier
    {
    protected:
        std::mutex mtx;
        std::queue<std::string> urls;
        virtual bool url_exists(const std::string& url) noexcept;

    public:
        frontier() noexcept {}
        frontier(const std::initializer_list<std::string>& il) noexcept;
        virtual ~frontier();
        virtual void add_url(const std::string& url) noexcept;
        virtual std::string get_next();
        virtual bool empty() const noexcept;
        virtual size_t size() const noexcept;
    };

    // Class with front and back queues.
    class mercator : public frontier
    {
    private:
        std::vector<std::queue<std::string>> front_queue;
        std::vector<std::queue<std::string>> back_queue;

    public:
        mercator() noexcept {}
        mercator(std::initializer_list<std::string>& il) noexcept;
        void add_url(const std::string& url, unsigned short priority) throw();
        bool empty() const noexcept override;
        std::string get_next() override;
        size_t size() const noexcept override;
    };

    // Exception class.
    class priority_exception: public std::exception
    {
    public:
        const char* what() const noexcept override;
    };
}
