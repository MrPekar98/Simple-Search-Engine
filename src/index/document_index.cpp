#include "document_index.hpp"
#include <utility>

namespace Pekar
{
    DocumentIndex DocumentIndex::create(const std::set<Location>& items)
    {
        DocumentIndex d;
        d.load(items);
        return d;
    }

    std::set<unsigned> DocumentIndex::load(const std::set<Location>& items)
    {
        std::set<unsigned> ks;

        for (auto it = items.cbegin(); it != items.cend(); it++)
        {
            ks.insert(add(*it));
        }

        this->k.insert(ks.cbegin(), ks.cend());
        return ks;
    }

    // Adds new data from a collection of items
    // If a document with the same ID already exists, the old document will be lost, but still exist on disk
    unsigned DocumentIndex::add(const Location& location)
    {
        this->idx[DocumentIndex::keyCounter] = location;
        this->k.insert(DocumentIndex::keyCounter);
        return DocumentIndex::keyCounter++;
    }

    void DocumentIndex::remove(const unsigned& key)
    {
        this->idx.erase(key);
        this->k.erase(key);
    }

    Location& DocumentIndex::find(const unsigned& key)
    {
        if (this->k.find(key) == this->k.end())
            throw "Not found";

        return this->idx[key];
    }

    const std::set<unsigned>& DocumentIndex::keys() const noexcept
    {
        return this->k;
    }
}