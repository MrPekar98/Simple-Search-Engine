#include "frontier.hpp"

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
        if (this->urls.empty() || !url_exists(url))
            this->urls.push(url);
    }

    // TODO: Use optional here.
    // Returns next URL in line and removes it.
    std::string frontier::get_next() noexcept
    {
        std::string next = this->urls.front();
        this->urls.pop();

        return next;
    }

    // Tells wether frontier is empty.
    bool frontier::empty() const noexcept
    {
        return this->urls.empty();
    }
}
