#ifndef DOCUMENT_INDEX_HPP
#define DOCUMENT_INDEX_HPP

#include "index.hpp"
#include "location.hpp"
#include "../document.hpp"
#include <string>
#include <map>

namespace Pekar
{
    // Indexing of ID to location on disk
    class DocumentIndex: public Index<unsigned, Location>
    {
    public:
        DocumentIndex() = default;
        DocumentIndex(const DocumentIndex& other) = default;
        DocumentIndex(DocumentIndex&& other) = default;
        DocumentIndex& operator=(const DocumentIndex& other) = default;
        DocumentIndex& operator=(DocumentIndex&& other) = default;
        static DocumentIndex create(const std::set<Location>& items);
        std::set<unsigned> load(const std::set<Location>& items) override;
        unsigned add(const Location& item) override;
        void remove(const unsigned& key) override;
        Location& find(const unsigned& key) override;
        const std::set<unsigned>& keys() const noexcept;

    private:
        std::map<unsigned, Location> idx;
        std::set<unsigned> k;
        unsigned keyCounter = 1;
    };
}

#endif