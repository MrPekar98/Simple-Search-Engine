#ifndef DOCUMENT_STORE_HPP
#define DOCUMENT_STORE_HPP

#include "../index/index.hpp"
#include "../index/location.hpp"
#include "../document.hpp"
#include <string>
#include <fstream>
#include <mutex>
#include <set>

namespace Pekar
{
    class DocumentStore: public Index<Location, Document>
    {
    public:
        DocumentStore() = delete;
        DocumentStore(const DocumentStore& other) = default;
        DocumentStore(DocumentStore&& other) = default;
        DocumentStore& operator=(const DocumentStore& other) = default;
        DocumentStore& operator=(DocumentStore&& other) = default;
        DocumentStore(const std::string& dataFile);
        std::set<Location> load(const std::set<Document>& items) override;
        Location add(const Document& item) override;
        void remove(const Location& key) override;
        Document& find(const Location& key) override;
        const std::set<Location>& getLocations() const noexcept;

    private:
        void loadFromFile(std::ifstream& file);
        void checkFile(const std::fstream& stream) const;
        std::string file;
        std::mutex mtx;
        std::set<Location> locations;
        Document tempDocument;
    };
}

#endif