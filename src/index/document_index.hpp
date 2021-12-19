#ifndef DOCUMENT_INDEX_HPP
#define DOCUMENT_INDEX_HPP

#include "index.hpp"
#include "location.hpp"
#include "../document.hpp"
#include <string>

namespace Pekar
{
    // Indexing of ID to location on disk
    class DocumentIndex: public Index<unsigned, Document>
    {
    public:
        DocumentIndex(const std::string& dataFile);
        void load(const std::set<Document>& items) noexcept override;
        template <typename Key, typename Item>
        void add(Key&& key, Item&& document) noexcept override;
        template <typename Key>
        void remove(Key&& key) noexcept override;
        template <typename Key>
        Document find(Key&& key) noexcept override;

    private:
        std::string dataFile;
        std::map<unsigned, Location> idx;
    };
}

#endif