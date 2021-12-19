#include "document_index.hpp"

#include <utility>

namespace Pekar
{
    // Constructor
    DocumentIndex::DocumentIndex(const std::string& dataFile)
        : dataFile(dataFile)
    {

    }

    void DocumentIndex::load(const std::set<Document>& items) noexcept
    {
        for (auto it = item.cbegin(); it != items.cend(); it++)
        {
            add(it->getId(), *it);
        }
    }

    // Adds new data from a collection of items
    // If a document with the same ID already exists, the old document will be lost, but still exist on disk
    template <typename Key, typename Item>
    void DocumentIndex::add(Key&& key, Item&& document) noexcept
    {
        //this->idx[std::forward<Key>(key)] = std::forward<Item>(location);
    }

    template <typename Key>
    void remove(Key&& key) noexcept
    {

    }

    template <typename Key>
    Document DocumentIndex::find(Key&& key) noexcept
    {

    }
}