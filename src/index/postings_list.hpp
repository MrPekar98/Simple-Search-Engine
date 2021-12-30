#ifndef POSTINGS_LIST_HPP
#define POSTINGS_LIST_HPP

#include "index.hpp"
#include "../document.hpp"
#include "document_index.hpp"
#include "../store/document_store.hpp"
#include <string>
#include <set>
#include <map>

namespace Pekar
{
    class PostingsList: public Index<std::string, std::set<Document>>
    {
    public:
        PostingsList(const std::string& dataFile);
        std::set<std::string> load(const std::set<std::set<Document>>& items) override;
        std::string add(const std::set<Document>& item) override;
        void remove(const std::string& key) override;
        std::set<Document>& find(const std::string& key) override;

    private:
        DocumentIndex dIdx;
        DocumentStore dStore;
        std::map<std::string, std::set<unsigned>> termToDocuments;
        std::set<Document> foundDocuments;
    };
}

#endif