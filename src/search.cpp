#include "store/document_store.hpp"

#include <iostream>

// TODO: Parse config.hpp to make sure all definitions required are there
int main()
{
    Pekar::DocumentStore ds("test_data");

    try
    {
        std::set<Pekar::Location> locations = ds.getLocations();

        for (auto it = locations.begin(); it != locations.end(); it++)
        {
            std::cout << it->getFileName() << it->getFileBlock().pos << " - " << it->getFileBlock().blockLength << std::endl;
        }

        Pekar::Document d = ds.find(Pekar::Location("test_data", Pekar::Block(33, 59)));
        std::cout << d.serialize() << std::endl;
    }

    catch (std::string& error)
    {
        std::cout << "Error: " << error << std::endl;
    }

    return 0;
}