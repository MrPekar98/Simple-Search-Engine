#include "config_parser.hpp"
#include "query/query.hpp"

#include <iostream>

// TODO: Parse config.hpp to make sure all definitions required are there
int main()
{
    try
    {
        parse();
        Pekar::Query q = Pekar::Query::make(std::string("Hello world"));
        std::cout << q.queryString() << std::endl;
    }

    catch (const std::system_error& error)
    {
        std::cerr << error.what() << std::endl;
    }

    return 0;
}