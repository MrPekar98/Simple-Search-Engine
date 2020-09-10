#include "crawler/crawler.hpp"
#include <vector>

#include <iostream>
#include <thread>
#include <chrono>

// Main function.
int main(int argc, char** argv)
{
    std::vector<scam::crawler::document> docs;
    std::vector<std::string> urls = {"https://ensk.dict.cc/"};

    std::thread t([&docs](){
        std::cout << "Main thread startet." << std::endl << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Documents: " << docs.size() << std::endl << std::endl;

        for (unsigned i = 0; i < docs.size(); i++)
        {
            std::cout << docs[i].url << std::endl;
            std::cout << docs[i].content << std::endl << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout << std::endl << "Documents: " << docs.size() << std::endl << std::endl;

        for (unsigned i = 0; i < docs.size(); i++)
        {
            std::cout << docs[i].url << std::endl;
            std::cout << docs[i].content << std::endl << std::endl;
        }
    });

    scam::crawler::crawl(urls, docs);
    t.join();

    return 0;
}
