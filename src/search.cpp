#include "config_parser.hpp"
#include "server/server.hpp"
#include <iostream>

int main()
{
    try
    {
        parse();
        
        Pekar::Server s(HOST, PORT, Pekar::ReturnType::TEXT, SEED);
        s.start(false);
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