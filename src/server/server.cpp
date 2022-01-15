#include "server.hpp"
#include "../crawler/crawler.hpp"
#include "../query/document_search.hpp"
#include "../query/query.hpp"
#include <cpprest/uri_builder.h>

#include <iostream>

namespace Pekar
{
    Server::Server(const std::string& host, const unsigned short& port, const ReturnType& returnType, const std::set<std::string> seedSet, const std::string& dbBackingFile)
        : detach(true), seed(seedSet), pl(dbBackingFile)
    {
        web::uri_builder uri = web::uri_builder().
                                    set_scheme("http").
                                    set_host(host).
                                    set_port(port);
        this->listener = web::http::experimental::listener::http_listener(uri.to_uri().to_string());
        this->listener.support(web::http::methods::GET, Server::getHandler);
        this->listener.support(web::http::methods::POST, Server::postHandler);
    }

    void Server::start(bool detached)
    {
        if (this->started)
            return;

        auto& index = this->pl;
        auto& seedSet = this->seed;
        this->crawler_thread = std::thread([&index, &seedSet](){ Pekar::crawl(seedSet, index, THREADS); });
        this->listener.open().wait();
        this->detach = detached;
        this->started = true;

        if (!detached)
            wait();
    }

    void Server::stop()
    {
        this->listener.close().wait();
        this->detach = true;
        this->started = false;
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

    void Server::getHandler(web::http::http_request request)
    {
        std::cout << "GET received" << std::endl;
        request.reply(web::http::status_codes::OK);
    }

    void Server::postHandler(web::http::http_request request)
    {
        request.reply(web::http::status_codes::OK);
    }
}