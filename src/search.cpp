#include "config_parser.hpp"
#include "query/query.hpp"
#include "query/ranker.hpp"
#include "index/postings_list.hpp"
#include "query/document_search.hpp"

#include <iostream>

// TODO: Parse config.hpp to make sure all definitions required are there
int main()
{
    try
    {
        parse();
        Pekar::Document d1("site1", "Here is some test content", std::set<std::string>({"google.com", "facebook.com"})), d2("site2", "content3", std::set<std::string>({"google.com", "facebook.com"})),
            d3("site3", "test some other hello", std::set<std::string>({"google.com", "facebook.com"}));

        Pekar::Query q = Pekar::Query::make("test");
        Pekar::PostingsList pl(std::string(DATA_FILE));
        //pl.add(std::set<Pekar::Document>({d1, d2, d3}));
        std::vector<Pekar::Document> searchResult = Pekar::DocumentSearch::search(q, pl, 0.5);

        for (const auto& d : searchResult)
        {
            std::cout << d.getUrl() << std::endl;
        }
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