#include <string.h>
#include <string>
#include <thread>
#include <mutex>
#include <curl/curl.h>
#include "crawler.hpp"
#include "shingles.hpp"
#include "data/frontier.hpp"
#include <cppw_htmlparser.h>
#include "../config.hpp"
#include <iostream>
#include <functional>
#include <chrono>

#define PRIORITIES 4
#define BACK_SZ 3

#if SIMPLE_FRONTIER
#define FRONTIER frontier
#else
#define FRONTIER mercator
#endif

namespace scam::crawler
{
    // CURL callbacks must be static member of a class.
    class curl_write
    {
    public:
        static size_t write_data(void* ptr, size_t size, size_t nmemb, void* buffer)
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

    // Private global mutex.
    static std::mutex mtx;

    // Prototypes.
    static void load_seed_set(FRONTIER& frontier, const std::set<std::string>& urls);
    static inline void join_threads(std::vector<std::thread*>& threads);
    static void log_session(CURL* handle);
    static CURL* handle_setup();
    static void curl_handle_write(std::vector<document>& documents, const std::string& host, const std::string& content, FRONTIER& urls);
    static std::string parse_content(const std::string& content);
    static bool analyse_content(const std::string& html_content, const std::vector<document>& documents);
    static std::set<std::string> extract_links(const std::string& html);
    static inline bool status_ok(CURL* handle);

    // Returns vector of document contents.
    // This function will potentially never terminate. Should therefore be called in another thread.
    void crawl(const std::set<std::string>& urls, std::vector<document>& result_documents)
    {
        // Seeding.
#if SIMPLE_FRONTIER
        FRONTIER url_frontier;
#else
        FRONTIER url_frontier(PRIORITIES, BACK_SZ);
#endif
        load_seed_set(url_frontier, urls);

#if THREADING
        std::vector<std::thread*> threads;

        for (unsigned i = 0; i < THREAD_COUNT; i++)
        {
            threads.push_back(new thread([&url_frontier, &result_documents](){
#endif
                CURL* handle = handle_setup();

                while (!url_frontier.empty())
                {
                    std::string buffer, url = url_frontier.get_next();

                    if (url.size() == 0)
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(4));
                        continue;
                    }

                    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
                    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, curl_write::write_data);
                    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buffer);
                    curl_easy_perform(handle);

                    if (!status_ok(handle))
                        continue;

#if DEBUG
                    log_session(handle);
#endif
                    curl_handle_write(result_documents, url, buffer, url_frontier);
                }
#if THREADING
            }));
        }

        join_threads(threads);
#endif
    }

    // Loads seed set into URL frontier.
    static void load_seed_set(FRONTIER& frontier, const std::set<std::string>& urls)
    {
        unsigned i = 0;

        for (std::set<std::string>::iterator it = urls.begin(); it != urls.end(); it++)
        {
#if SIMPLE_FRONTIER
            frontier.add_url(*it);
#else
            frontier.add_url(*it, i++ % PRIORITIES);
#endif
        }
    }

    // Joins all threads in vector.
    static inline void join_threads(std::vector<std::thread*>& threads)
    {
        for (unsigned i = 0; i < threads.size(); i++)
        {
            threads[i]->join();
        }
    }

    // Logs connection information.
    static void log_session(CURL* handle)
    {
        long response_code;
        char* host = NULL;
        curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &host);

#if THREADING
        mtx.lock();
#endif
        std::cout << "Response code: " << response_code << std::endl;

        if (host)
            std::cout << "Host: " << host << std::endl << std::endl;
#if THREADING
        mtx.unlock();
#endif
    }

    // CURL crawling setup.
    static CURL* handle_setup()
    {
        CURL* handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(handle, CURLOPT_USERAGENT, USER_AGENT);
        curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
        curl_easy_setopt(handle, CURLOPT_PROXYAUTH, CURLAUTH_ANY);

        return handle;
    }

    // Handles CURL write.
    static void curl_handle_write(std::vector<document>& documents, const std::string& host, const std::string& content, FRONTIER& urls)
    {
#if FP_CHECK
        if (!analyse_content(content, documents))
            return;
#endif

        std::set<std::string> extracted_links = extract_links(content);
        unsigned i = 0;

        for (std::set<std::string>::iterator it = extracted_links.begin(); it != extracted_links.end(); it++)
        {
#if SIMPLE_FRONTIER
            urls.add_url(*it);
#else
            urls.add_url(*it, i++ % 4);
#endif
        }

        documents.push_back(document(host, parse_content(content), extracted_links));
    }

    // Parses HTML.
    static std::string parse_content(const std::string& content)
    {
#if PARSE_HTML
        std::string error_msg;
        CWNode tree;

        if (!tree.ParseHTML(content, &error_msg))
            return "";

#if TITLE_ONLY
        CWNode* node = tree.FindChildByName("title", false, true);
#else
        CWNode* node = tree.FindChildByName("body", false, true);
#endif

        if (!node)
            return "";

        return node->GetText();
#else
        return content;
#endif
    }

    // Analyses contents using shingles. Returns false is the content does not pass the threshold.
    static bool analyse_content(const std::string& content, const std::vector<document>& documents)
    {
        if (shingles(content, SHING_LEN).size() == 0)
            return false;

        unsigned length = documents.size();

        for (int i = 0; i < length; i++)
        {
            if (shingles(content, SHING_LEN).size() == 0)
                continue;

            else if (jaccard(shingles(content, SHING_LEN), shingles(documents[i].content, SHING_LEN)) >= JACC_THRES)
                return false;
        }

        return true;
    }

    // Extracts hyperlinks from HTML content.
    static std::set<std::string> extract_links(const std::string& html)
    {
        std::set<std::string> links;
        unsigned length = html.length();

        for (unsigned i = 0; i < length; i++)
        {
            if (html.substr(i, 4).compare("href") == 0 && html.substr(i + 6, 4).compare("http") == 0)
            {
                std::string startstr = html.substr(i + 6);
                links.insert(startstr.substr(0, startstr.find_first_of('"')));
                i = startstr.find_first_of('"') + i;
            }
        }

        return links;
    }

    // Checks that status code from connection is 200.
    // The handle must have been performed before calling this function.
    static inline bool status_ok(CURL* handle)
    {
        long code;
        curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &code);

        return code == 200;
    }
}
