#ifndef FRONTER_HPP
#define FRONTIER_HPP

#include <string>
#include <vector>
#include <mutex>

namespace Pekar
{
    class AbstractFrontier
    {
    public:
        virtual ~AbstractFrontier() {}
        void add(const std::string& url) noexcept;
        bool empty() const noexcept;
        std::string next() noexcept;

    protected:
        virtual void performAdd(const std::string& url) noexcept = 0;
        virtual bool performEmpty() const noexcept = 0;
        virtual std::string performNext() noexcept = 0;
    };

    // Simple LIFO frontier
    class SimpleFrontier: public AbstractFrontier
    {
    protected:
        void performAdd(const std::string& url) noexcept override;
        bool performEmpty() const noexcept override;
        std::string performNext() noexcept override;

    private:
        std::mutex mtx;
        std::vector<std::string> urls;
    };
}

#endif