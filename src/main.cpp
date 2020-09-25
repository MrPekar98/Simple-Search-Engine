#include "crawler/crawler.hpp"
#include "crawler/data/postings_list.hpp"
#include "config.hpp"
#include <vector>
#include <set>
#include <thread>
#include <chrono>
#include <iostream>

// Prototype.
void display_search(const std::string& query, const scam::indexing::postings_list& postings);

// Main function.
int main(int argc, char** argv)
{
    std::vector<scam::crawler::document> docs;
    scam::indexing::postings_list list(docs);
    std::set<std::string> urls({"https://www.aau.dk/",
                                "https://math.stackexchange.com/?newreg=a6a27149f0eb471abc90dcc138ea8b26",
                                "https://softwareengineering.stackexchange.com/",
                                "https://bundesligastreaming.com/live/borussia-dortmund-livestream-15/channel-1/",
                                "https://www.google.com/search?sxsrf=ALeKk00An4c6i3Xpzh87DclbYT3U1P65iw%3A1599832982233&source=hp&ei=loNbX-y7C4GzkwWj0orYAw&q=Elisabeth+Niemeyer+Laursen&oq=Elisabeth+Niemeyer+Laursen&gs_lcp=CgZwc3ktYWIQAzIHCCEQChCgAToCCAA6AgguOgUILhCxAzoFCAAQsQM6BQguEJMCOgQIABAKOgUILhDLAToFCAAQywE6CAgAEBYQChAeOgYIABAWEB46CAgAEA0QChAeOgYIABANEB46CAgAEA0QBRAeOgUIIRCgAToECAAQDVDnAljLLWCXLmgCcAB4BoABygOIAaEukgEJNy43LjQuNi4zmAEAoAEBqgEHZ3dzLXdpeg&sclient=psy-ab&ved=0ahUKEwjs3umkouHrAhWB2aQKHSOpAjsQ4dUDCAc&uact=5",
                                "https://www.google.com/search?sxsrf=ALeKk03ZA9qKWAqF8Yh71_9z6kzRyQhGIg%3A1599832989300&ei=nYNbX8_cEcOcsAfLl7nIDA&q=Melanie+Selman&oq=Melanie+Selman&gs_lcp=CgZwc3ktYWIQAzIFCAAQywEyBQgAEMsBMgYIABAWEB4yBggAEBYQHjoECCMQJzoFCAAQsQM6AggAOgQIABBDOgQILhBDOgoILhCxAxBDEJMCOgcILhCxAxBDOgUILhCxAzoICC4QsQMQgwE6CAgAELEDEIMBOggILhCxAxCTAjoCCC46CwguELEDEIMBEJMCOgQILhAKOgUILhCTAjoFCC4QywE6BwgAEAoQywE6CAgAEBYQChAeUOq2AViExgFg9sYBaABwAHgAgAHpAogB5Q-SAQgxMy4xLjAuMpgBAKABAaoBB2d3cy13aXrAAQE&sclient=psy-ab&ved=0ahUKEwiPn5uoouHrAhVDDuwKHctLDskQ4dUDCA0&uact=5",
                                "https://www.google.com/search?sxsrf=ALeKk03NLkYAIqt5nmceCcrjDMRJPdyaWQ%3A1599833015789&ei=t4NbX_LpL8zXkwWo44L4DQ&q=Abiram+Mohanaraj&oq=Abiram+Mohanaraj&gs_lcp=CgZwc3ktYWIQAzIFCCEQoAEyBQghEKABOgQIIxAnOgUIABDLAToGCAAQFhAeOggILhCxAxCDAToFCC4QsQM6BAguEAo6AgguOgIIADoHCCMQ6gIQJzoHCC4Q6gIQJzoECAAQQzoICAAQsQMQgwE6BQgAELEDOgQILhBDOgcILhBDEJMCOgUILhDLAToHCAAQChDLAToICAAQFhAKEB46BggAEAoQEzoHCCEQChCgAVCmwQFY0PkBYLH7AWgCcAB4AIABlQGIAYsPkgEEMTAuOZgBAKABAaoBB2d3cy13aXqwAQrAAQE&sclient=psy-ab&ved=0ahUKEwiyoey0ouHrAhXM66QKHaixAN8Q4dUDCA0&uact=5",
                                "https://www.google.com/search?sxsrf=ALeKk00kyTsMg9r18A7slpJeu7DVDMBXyA%3A1599833049416&ei=2YNbX_n1GJGzsAflz6GYDw&q=Kristian+Gregersen&oq=Kristian+Gregersen&gs_lcp=CgZwc3ktYWIQAzICCAAyBggAEBYQHjIGCAAQFhAeMgYIABAWEB4yBggAEBYQHjIGCAAQFhAeMgYIABAWEB4yBggAEBYQHjIGCAAQFhAeMgYIABAWEB46BAgjECc6BQgAEMsBOgcIABAKEMsBOgcIIxDqAhAnOgcILhDqAhAnOgUIABCxAzoFCC4QsQM6CAgAELEDEIMBOgQIABAKOgIILjoECC4QCjoHCC4QChCTAjoFCC4QkwI6BQguEMsBOgQIABANOgYIABANEB46CAgAEA0QBRAeOggIABAIEA0QHlDtb1jfrQFgxK8BaAdwAHgAgAFwiAHSEZIBBDIzLjOYAQCgAQGqAQdnd3Mtd2l6sAEKwAEB&sclient=psy-ab&ved=0ahUKEwi5xvDEouHrAhWRGewKHeVnCPMQ4dUDCA0&uact=5",
                                "https://www.google.com/search?sxsrf=ALeKk03MGwUi-P4TH493wyO_jzeZLkX-8g%3A1599833091588&ei=A4RbX6WxI4bmsAfw5o6gCA&q=Martin+pekar+Christensen&oq=Martin+pekar+Christensen&gs_lcp=CgZwc3ktYWIQAzoECCMQJzoECC4QQzoECAAQQzoKCC4QsQMQgwEQQzoFCAAQsQM6BQguELEDOgIIADoICC4QsQMQgwE6BQguEJMCOgIILjoECAAQCjoFCAAQywE6BwguEAoQywE6BggAEBYQHjoFCCEQoAE6BwghEAoQoAFQ82FY5Xpg6HxoAXAAeACAAbgBiAGWFJIBBDIxLjWYAQCgAQGqAQdnd3Mtd2l6wAEB&sclient=psy-ab&ved=0ahUKEwjlvv7YouHrAhUGM-wKHXCzA4QQ4dUDCA0&uact=5"});

    std::thread crawling_thread([&urls, &docs](){
        scam::crawler::crawl(urls, docs);
        std::cout << "Crawling has terminated." << std::endl;
    });

    std::thread indexing_thread([&list, &docs](){
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(INDEX_UPDATE_TIME));
            list.reload(docs);
        }
    });

    std::string query;
    std::this_thread::sleep_for(std::chrono::seconds(INDEX_UPDATE_TIME));

    while (true)
    {
        std::cout << ": ";
        std::cin >> query;
        display_search(query, list);

        std::cout << std::endl;
    }

    crawling_thread.join();
    indexing_thread.join();

    return 0;
}

// Displays search result.
void display_search(const std::string& query, const scam::indexing::postings_list& postings)
{
    std::vector<scam::crawler::document> documents = postings.search(query);
    unsigned length = documents.size();

    for (unsigned i = 0; i < length; i++)
    {
#if TITLE_ONLY
        std::cout << documents[i].content << std::endl;
#endif
        std::cout << documents[i].url << std::endl << std::endl;
    }
}