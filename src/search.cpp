#include "config_parser.hpp"
#include "query/query.hpp"
#include "query/ranker.hpp"
#include "index/postings_list.hpp"

#include <iostream>

// TODO: Parse config.hpp to make sure all definitions required are there
int main()
{
    try
    {
        parse();
        Pekar::Query q = Pekar::Query::make(std::string("Hello test"));
        Pekar::Document d1("site1", "Here is some test content", std::set<std::string>({"google.com", "facebook.com"})), d2("site2", "content3", std::set<std::string>({"google.com", "facebook.com"})),
            d3("site3", "test some other hello", std::set<std::string>({"google.com", "facebook.com"}));
        std::vector<Pekar::Document> docs = {d1, d2, d3};
        std::vector<Pekar::Document> ranked = Pekar::Ranker::rank(q, docs, 0.1);

        for (auto it = ranked.cbegin(); it != ranked.cend(); it++)
        {
            std::cout << " - " << it->getUrl() << std::endl;
        }

        Pekar::PostingsList pl(std::string(DATA_PATH) + std::string(DATA_FILE));
        //pl.add(std::set<Pekar::Document>({d1, d2, d3}));
        std::set<Pekar::Document> docsFound = pl.find("test");

        for (const auto& d : docsFound)
        {
            std::cout << d.getUrl() << std::endl;
        }
    }

    catch (const std::system_error& error)
    {
        std::cerr << error.what() << std::endl;
    }

    return 0;
}