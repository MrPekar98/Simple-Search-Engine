#include "server.hpp"
#include "render_page.hpp"
#include "../crawler/crawler.hpp"
#include "../query/document_search.hpp"
#include "../query/query.hpp"
#include <cpprest/uri_builder.h>
#include <fstream>

#define KILOBYTE 1024
#define MEGABYTE 1048576
#define GIGABYTE 1073741824

namespace Pekar
{
    static PostingsList index(DATA_FILE);
    static inline std::string parsePostString(const std::string& str);
    static unsigned long fileSize(const std::string& file);

    Server::Server(const std::string& host, const unsigned short& port, const ReturnType& returnType, const std::set<std::string> seedSet)
        : detach(true), seed(seedSet)
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

        auto& seedSet = this->seed;
        this->crawler_thread = std::thread([&seedSet](){ Pekar::crawl(seedSet, index, THREADS); });
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

    std::string Server::toStr(const std::vector<Document>& docs) noexcept
    {
        std::string str = "";

        for (const Document& d : docs)
        {
            str += d.getUrl() + "\n";
        }

        return str;
    }

    void Server::getHandler(web::http::http_request request)
    {
        using namespace web::http;
        long dataConsumption = fileSize(DATA_FILE);
        std::string dataConsumptionStr = std::to_string(dataConsumption) + (dataConsumption < KILOBYTE ? " B": dataConsumption < KILOBYTE ? " KB" : dataConsumption < MEGABYTE ? " MB" : " GB");
        static std::string homepage = Render::render(HOMEPAGE, std::to_string(THREADS), std::to_string(SHINGLES),
                                        HOST, std::to_string(PORT), std::to_string(index.documentCount()), dataConsumptionStr);
        http_response response(status_codes::OK);
        response.set_body(homepage, utf8string("text/html; charset=UTF-8"));

        request.reply(response);
    }

    void Server::postHandler(web::http::http_request request)
    {
        using namespace web::http;
        http_response response(status_codes::OK);
        auto body = request.body();
        std::string queryStr = "";

        while (!body.is_eof())
        {
            char c = body.read().get();
            queryStr += c;
        }

        Query q = Query::make(parsePostString(queryStr));
        std::vector<Document> results = DocumentSearch::search(q, index, SIM_THRES);
        
        response.set_body(toStr(results));
        request.reply(response);
    }

    static inline std::string parsePostString(const std::string& str)
    {
        size_t start = str.find('=') + 1, end = str.find("submit", start) - 1;
        std::string cutStr = str.substr(start, end - start);
        
        for (unsigned i = 0; i < cutStr.length(); i++)
        {
            if (cutStr[i] == '+')
            {
                cutStr[i] = ' ';
                i--;
            }

            else if (cutStr[i] == '%')
            {
                cutStr.erase(i, cutStr.find('+', i) - i);
                i--;
            }
        }

        return cutStr;
    }

    // Size of document in bytes
    static unsigned long fileSize(const std::string& file)
    {
        std::ifstream stream(file);

        if (!stream)
            return 0;

        auto current = stream.tellg();
        stream.seekg(0, std::ios::end);
        
        auto end = stream.tellg();
        stream.seekg(current, std::ios::beg);

        return end;
    }
}