#include "frontier.hpp"

namespace Pekar
{
    void AbstractFrontier::add(const std::string& url) noexcept
    {
        performAdd(url);
    }

    bool AbstractFrontier::empty() const noexcept
    {
        return performEmpty();
    }

    std::string AbstractFrontier::next() noexcept
    {
        return performNext();
    }

    void SimpleFrontier::performAdd(const std::string& url) noexcept
    {
        this->mtx.lock();
        this->urls.push_back(url);
        this->mtx.unlock();
    }

    bool SimpleFrontier::performEmpty() const noexcept
    {
        return this->urls.size() == 0;
    }

    std::string SimpleFrontier::performNext() noexcept
    {
        this->mtx.lock();
        std::string url = this->urls[this->urls.size() - 1];
        this->urls.erase(this->urls.cend() - 1);
        this->mtx.unlock();

        return url;
    }
}