#include <string.h>
#include <string>
#include <thread>
#include <curl/curl.h>
#include "crawler.hpp"
#include "data/frontier.hpp"

namespace crawler
{
    // Prototypes.
    static void load_seed_set(frontier& f, const std::vector<std::string>& urls);
    static CURL* handle_setup(const std::string& url);

    // Returns vector of document contents. This function will potentially never terminate.
    std::vector<std::string> crawl(const std::vector<std::string>& urls)
    {
        std::vector<std::string> documents;

        // Seeding.
        frontier url_frontier;
        load_seed_set(url_frontier, urls);

        while (!url_frontier.empty())
        {
            std::thread([&url_frontier](){
                 CURL* handle = handle_setup(url_frontier.get_next());
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, [](char* ptr, size_t size, size_t memb, void* userdata){                    char* buffer = new char[memb + 1];
                    memcpy(buffer, ptr, memb);
                    buffer[memb] = '\0';
                });

                curl_easy_perform(handle);
                curl_easy_cleanup(handle);
            });
        }

        return documents;
    }

    // Loads seed set into URL frontier.
    static void load_seed_set(frontier& f, const std::vector<std::string>& urls)
    {
        for (int i = 0; i < urls.size(); i++)
        {
            f.add_url(urls[i]);
        }
    }

    // CURL crawling setup.
    static CURL* handle_setup(const std::string& url)
    {
        CURL* handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

        return handle;
    }

    // Returns vector of document contents with limit of collection.
    std::vector<std::string> crawl(const std::vector<std::string>& urls, unsigned long doc_limit)
    {
        return crawl(urls);
    }
}
