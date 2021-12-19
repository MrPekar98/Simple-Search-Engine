#include "document.hpp"

#include <iostream>

int main()
{
    Pekar::Document d = Pekar::Document::deserialize("1###google.com###Here is some content###facebook.com,microsoft.com,twitter.com###");
    std::cout << d.getId() << std::endl << d.getUrl() << std::endl << d.getContent() << std::endl;

    for (auto it = d.cbegin(); it != d.cend(); it++)
    {
        std::cout << *it << std::endl;
    }

    return 0;
}