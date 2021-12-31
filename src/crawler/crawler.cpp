#include "crawler.hpp"
#include "frontier.hpp"
#include "../config.hpp"
#include "../tokenizer.hpp"
#include "html/html_parser.hpp"
#include <curl/curl.h>
#include <thread>
#include <vector>

#include <iostream>

namespace Pekar
{
    class CurlWrite
    {
    public:
        // buffer must be of type std::string
        static size_t writeData(void* ptr, size_t size, size_t nmemb, void* buffer)
        {
            std::string* str = ((std::string*) buffer);
            str->append((char*) ptr, nmemb);

            for (char& c : *str)
            {
                if (c == 13)
                    c = 32;
            }

            return nmemb;
        }
    };

    static void singleCrawler(AbstractFrontier& frontier, PostingsList& pl, CURL* handle);
    static CURL* getHandle();
    static void loadFrontier(const std::set<std::string>& seedSet, AbstractFrontier& frontier);
    static inline bool checkStatus(CURL* handle);
    static bool fingerprintCheck(const std::string& content, const PostingsList& pl);
    static std::set<std::string> extractLinks(const std::string& content, const std::string& baseUrl);
    static inline std::string concatStrings(const std::vector<std::string>& strings);
    static inline std::string termString(const std::vector<Term>& terms);

    void crawl(const std::set<std::string>& seedSet, PostingsList& pl, const unsigned& threadCount) noexcept
    {
        SimpleFrontier frontier;
        std::vector<std::thread> threads;
        CURL* handle = getHandle();
        loadFrontier(seedSet, frontier);

        for (unsigned i = 0; i < threadCount; i++)
        {
            threads.push_back(std::thread([&frontier, &pl, &handle](){ singleCrawler(frontier, pl, handle); }));
        }

        for (std::thread& t : threads)
        {
            t.join();
        }
    }

    static void singleCrawler(AbstractFrontier& frontier, PostingsList& pl, CURL* handle)
    {
        while (!frontier.empty())
        {
            std::string buffer, url = frontier.next();
            
            curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buffer);
            curl_easy_perform(handle);

            if (!checkStatus(handle) || !fingerprintCheck(buffer, pl))
                continue;

            std::set<std::string> links = extractLinks(buffer, url);
            BaseHtmlParser parser(buffer);

#if STORE_CONTENT
            pl.add(std::set<Document>({Document(url, termString(Tokenizer::tokenize(concatStrings(parser.paragraphs()))), links)}));
#else
            pl.add(std::set<Document>({Document(url, termString(Tokenizer::tokenize(parser.title())), links)}));
#endif

            for (const std::string& link : links)
            {
                frontier.add(link);
            }
        }
    }

    static CURL* getHandle()
    {
        CURL* handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(handle, CURLOPT_USERAGENT, USER_AGENT);
        curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
        curl_easy_setopt(handle, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, CurlWrite::writeData);

        return handle;
    }

    static void loadFrontier(const std::set<std::string>& seedSet, AbstractFrontier& frontier)
    {
        for (const std::string url : seedSet)
        {
            frontier.add(url);
        }
    }

    static inline bool checkStatus(CURL* handle)
    {
        long code;
        curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &code);

        return code == 200;
    }

    // Return false if content is too similar to another document according to Jaccard coefficient
    static bool fingerprintCheck(const std::string& content, const PostingsList& pl)
    {
        return true;
    }

    static std::set<std::string> extractLinks(const std::string& content, const std::string& baseUrl)
    {
        std::set<std::string> links;
        size_t start = 0, end = 0;

        while (end != std::string::npos)
        {
            start = content.find("http", end);

            if (start == std::string::npos)
                break;

            end = content.find("\"", start + 8);
            links.insert(content.substr(start, end - start));
            std::cout << content.substr(start, end - start) << std::endl;
        }

        return links;
    }

    static inline std::string concatStrings(const std::vector<std::string>& strings)
    {
        std::string concat = "";

        for (const auto& str : strings)
        {
            concat += " " + str;
        }

        return concat;
    }

    static inline std::string termString(const std::vector<Term>& terms)
    {
        std::string str = "";

        for (const Term& t : terms)
        {
            str += t.getTerm() + " ";
        }

        return str.erase(str.length() - 1);
    }
}