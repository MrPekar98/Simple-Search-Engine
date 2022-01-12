#include "server.hpp"
#include "../crawler/crawler.hpp"
#include "../query/document_search.hpp"
#include "../query/query.hpp"
#include <cpprest/uri_builder.h>

namespace Pekar
{
    Server::Server(const std::string& host, const unsigned short& port, const ReturnType& returnType, const std::set<std::string> seedSet, const std::string& dbBackingFile)
        : detach(true), seed(seedSet), pl(dbBackingFile)
    {
        web::uri_builder uri = web::uri_builder().
                                    set_scheme("https").
                                    set_host(host).
                                    set_port(port).
                                    set_path(Server::toStr(returnType));
        this->listener = web::http::experimental::listener::http_listener(uri.to_uri().to_string());
        this->listener.support(Server::handler);
    }

    void Server::start(bool detached)
    {
        auto& index = this->pl;
        auto& seedSet = this->seed;
        this->crawler_thread = std::thread([&index, &seedSet](){ Pekar::crawl(seedSet, index, THREADS); });
        this->listener.open().wait();

        if (!detached)
            wait();
    }

    void Server::stop()
    {
        this->listener.close().wait();
        this->detach = true;
    }

    void Server::wait() const noexcept
    {
        while (!this->detach);
    }

    std::string Server::toStr(const ReturnType& rt) noexcept
    {
        switch (rt)
        {
            case ReturnType::JSON:
                return "json";

            case ReturnType::TEXT:
                return "text";

            default:
                return "text";
        }
    }

    void Server::handler(web::http::http_request request)
    {

    }
}