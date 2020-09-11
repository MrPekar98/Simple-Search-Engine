#include "frontier.hpp"
#include <functional>

namespace scam::crawler
{
    // Frontier destructor.
    frontier::~frontier() {}

    // Checks whether URL already exists in queue.
    bool frontier::url_exists(const std::string& url) noexcept
    {
        bool ret = false;

        for (int i = 0; i < this->urls.size(); i++)
        {
            std::string temp = this->urls.front();

            if (temp.compare(url) == 0)
                ret = true;

            this->urls.pop();
            this->urls.push(temp);
        }

        return ret;
    }

    // Adds URL if not already added.
    void frontier::add_url(const std::string& url) noexcept
    {
        this->mtx.lock();

        if (this->urls.empty() || !url_exists(url))
            this->urls.push(url);

        this->mtx.unlock();
    }

    // Returns next URL in line and removes it.
    std::string frontier::get_next() throw()
    {
        this->mtx.lock();

        if (empty())
        {
            this->mtx.unlock();
            throw std::bad_function_call();
        }

        std::string next = this->urls.front();
        this->urls.pop();
        this->mtx.unlock();

        return next;
    }

    // Tells wether frontier is empty.
    bool frontier::empty() const noexcept
    {
        return this->urls.empty();
    }

    // Add url into front queue.
    void mercator::add_url(const std::string& url, unsigned short priority) throw()
    {
        if (priority < 0 || priority > this->front_queue.size() + 1)
            throw priority_exception();
    }

    // Checks for frontier being empty.
    bool mercator::empty() const noexcept
    {
        return false;
    }

    // Gets next URL from back queue. Fills up a queue from back queue if empty.
    std::string mercator::get_next() throw()
    {
        return "Not implemented";
    }

    // Overridden exception class.
    const char* priority_exception::what() const noexcept
    {
        return "Bad priority";
    }
}
