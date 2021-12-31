#include "config_parser.hpp"
#include "query/query.hpp"
#include "index/postings_list.hpp"
#include "query/document_search.hpp"
#include "crawler/crawler.hpp"
#include <iostream>
#include <thread>

// This is temporary. A server should be setup instead.
int main()
{
    try
    {
        parse();
        Pekar::PostingsList pl(std::string(DATA_FILE));
        std::thread t([&pl](){ Pekar::crawl(std::set<std::string>({"https://www.apple.com/", "https://www.microsoft.com/"}), pl, THREADS); });
    
        while (true)
        {
            std::string input;
            std::cout << ": ";
            std::cin >> input;

            Pekar::Query q = Pekar::Query::make(input);
            std::vector<Pekar::Document> searchResult = Pekar::DocumentSearch::search(q, pl, MIN_RANK_SCORE);
            std::cout << std::endl;

            for (const auto& d : searchResult)
            {
                std::cout << d.getUrl() << std::endl;
            }

            std::cout << std::endl << std::endl;
        }

        t.join();
    }

    catch (const char* err)
    {
        std::cerr << err << std::endl;
    }

    catch (const std::system_error& error)
    {
        std::cerr << error.what() << std::endl;
    }

    return 0;
}