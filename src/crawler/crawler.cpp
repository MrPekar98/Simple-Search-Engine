#include "crawler.hpp"
#include "frontier.hpp"
#include "../config.hpp"
#include "../tokenizer.hpp"
#include "html/html_parser.hpp"
#include <curl/curl.h>
#include <thread>
#include <vector>
#include <similarity/jaccard.hpp>

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
    static bool fingerprintCheck(const Document& d, PostingsList& pl);
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

            if (!checkStatus(handle))
            {
                pl.remove(url);
                continue;
            }

            BaseHtmlParser parser(buffer);
            std::set<std::string> links = parser.links();

#if STORE_CONTENT
            std::string content = termString(Tokenizer::tokenize(concatStrings(parser.paragraphs())));
#else
            std::string content = termString(Tokenizer::tokenize(parser.title()));
#endif

            Document d(url, content, links);

            if (!fingerprintCheck(d, pl))
                continue;

            pl.add(std::set<Document>({d}));

            for (const std::string& link : links)
            {
                if (link != url)
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
    static bool fingerprintCheck(const Document& d, PostingsList& pl)
    {
        std::set<Document> docs = pl.all();
        Shingle s1 = Shingle::buildShingles(d.getContent(), SHINGLES);

        for (const Document& doc : docs)
        {
            if (d.getUrl() == doc.getUrl())
                return false;

            Shingle s2 = Shingle::buildShingles(doc.getContent(), SHINGLES);

            if (Jaccard::jaccard(s1.getShingles(), s2.getShingles()) > SIM_THRES)
                return false;
        }

        return true;
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