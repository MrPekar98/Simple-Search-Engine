#ifndef SERVER_HPP
#define SERVER_HPP

#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>
#include <cpprest/http_msg.h>
#include <thread>
#include <string>
#include <set>
#include "../index/postings_list.hpp"
#include "../config.hpp"

namespace Pekar
{
    enum class ReturnType
    {
        JSON,
        TEXT
    };

    class Server
    {
    public:
        Server(const std::string& host, const unsigned short& port, const ReturnType& returnType, const std::set<std::string> seedSet);
        Server(const Server& other) = delete;
        Server(Server&& other) = delete;
        Server& operator=(const Server& other) = delete;
        Server& operator=(Server&& other) = delete;
        void start(bool detached = true);
        void stop();
        void wait() const noexcept;

    private:
        static std::string toStr(const ReturnType& rt) noexcept;
        static std::string toStr(const std::vector<Document>& docs) noexcept;
        static void getHandler(web::http::http_request request);
        static void postHandler(web::http::http_request request);
        //static PostingsList index;
        const std::set<std::string> seed;
        std::thread crawler_thread;
        web::http::experimental::listener::http_listener listener;
        bool detach, started = false;
    };
}

#endif