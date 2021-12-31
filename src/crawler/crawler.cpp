#include "crawler.hpp"
#include "frontier.hpp"
#include "../config.hpp"
#include <curl/curl.h>
#include <cppw_htmlparser.h>
#include <thread>
#include <vector>

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
    static std::string getTitle(const std::string& content);

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

#if STORE_CONTENT
            pl.add(std::set<Document>({Document(url, buffer, links)}));
#else
            pl.add(std::set<Document>({Document(url, getTitle(buffer), links)}));
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
        unsigned length = content.length();

        for (unsigned i = 0; i < length; i++)
        {
            if (content.substr(i, 4).compare("href") == 0 && content.substr(i + 6, 4).compare("http") == 0)
            {
                std::string startstr = content.substr(i + 6);
                std::string link = startstr.substr(0, startstr.find_first_of('"'));

                if (link != baseUrl)
                    links.insert(link);
                
                i = startstr.find_first_of('"') + i;
            }
        }

        return links;
    }

    static std::string getTitle(const std::string& content)
    {
        CWNode tree;
        std::string error;
        tree.ParseHTML(content, &error);

        return tree.FindChildByName("title", false, true)->GetText();
    }
}