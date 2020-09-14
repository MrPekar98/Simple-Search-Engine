#include <set>
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
    static void load_seed_set(mercator& frontier, const std::vector<std::string>& urls);
    static inline void join_threads(std::vector<std::thread*>& threads);
    static void log_session(CURL* handle);
    static CURL* handle_setup(const std::string& url);
    static void curl_handle_write(std::vector<document>& documents, const std::string& host, const std::string& content, frontier& urls);
    static bool analyse_content(const std::string& html_content, const std::vector<document>& documents);
    static std::set<std::string> html_shingleterm_set(const std::string& html, unsigned short shingle_length);
    static std::set<std::string> extract_links(const std::string& html);

    // Returns vector of document contents.
    // This function will potentially never terminate. Should therefore be called in another thread.
    void crawl(const std::vector<std::string>& urls, std::vector<document>& result_documents)
    {
        // Seeding.
        mercator url_frontier(4, 3);
        load_seed_set(url_frontier, urls);

#if THREADING
        std::vector<std::thread*> threads;

        for (unsigned i = 0; i < THREAD_COUNT; i++)
        {
            threads.push_back(new thread([&url_frontier, &result_documents](){
#endif
                while (!url_frontier.empty())
                {
                    std::string buffer, url = url_frontier.get_next();

                    if (url.size() == 0)
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(4));
                        continue;
                    }

                    CURL* handle = handle_setup(url);
                    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, curl_write::write_data);
                    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buffer);
                    curl_easy_perform(handle);

#if DEBUG
                    log_session(handle);
#endif

                    curl_handle_write(result_documents, url, buffer, url_frontier);
                    curl_easy_cleanup(handle);
                }
#if THREADING
            }));
        }

        join_threads(threads);
#endif
    }

    // Loads seed set into URL frontier.
    static void load_seed_set(mercator& frontier, const std::vector<std::string>& urls)
    {
        for (int i = 0; i < urls.size(); i++)
        {
            frontier.add_url(urls[i], i % 4);
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
    static CURL* handle_setup(const std::string& url)
    {
        CURL* handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(handle, CURLOPT_USERAGENT, USER_AGENT);
        curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
        curl_easy_setopt(handle, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

        return handle;
    }

    // Handles CURL write.
    static void curl_handle_write(std::vector<document>& documents, const std::string& host, const std::string& content, frontier& urls)
    {
        if (!analyse_content(content, documents))
            return;

        std::set<std::string> extracted_links = extract_links(content);

        for (std::set<std::string>::iterator it = extracted_links.begin(); it != extracted_links.end(); it++)
        {
            urls.add_url(*it);
        }

        documents.push_back(document(host, content));
    }

    // Analyses contents using shingles. Returns false is the content does not pass the threshold.
    static bool analyse_content(const std::string& html_content, const std::vector<document>& documents)
    {
        if (html_shingleterm_set(html_content, SHING_LEN).size() == 0)
            return false;

        for (unsigned i = 0; i < documents.size(); i++)
        {
            if (html_shingleterm_set(documents[i].content, SHING_LEN).size() == 0)
                continue;

            if (jaccard(html_shingleterm_set(html_content, SHING_LEN), html_shingleterm_set(documents[i].content, SHING_LEN)) >= JACC_THRES)
                return false;
        }

        return true;
    }

    // Converts HTML content into set instance of terms.
    static std::set<std::string> html_shingleterm_set(const std::string& html, unsigned short shingle_length)
    {
#if PARSE_HTML
        std::string error_msg;
        CWNode tree;
        std::set<std::string> shingle_set;

        // TODO: This is unfortunately true.
        if (!tree.ParseHTML(html, &error_msg))
            return shingle_set;

        CWNode* body = tree.FindChildByName("body", false, true);

        if (!body)
            return shingle_set;

        std::string body_text = body->GetText();
        delete body;

        return shingles(body_text, shingle_length);
#else
        return shingles(html, shingle_length);
#endif
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
}
