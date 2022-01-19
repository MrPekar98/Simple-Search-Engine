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
        PostingsList() = delete;
        PostingsList(const PostingsList& other) = delete;
        PostingsList(PostingsList&& other) = delete;
        PostingsList& operator=(const PostingsList& other) = delete;
        PostingsList& operator=(PostingsList&& other) = delete;
        std::set<std::string> load(const std::set<std::set<Document>>& items) override;
        std::string add(const std::set<Document>& item) override;
        void remove(const std::string& key) override;
        std::set<Document>& find(const std::string& key) override;
        const std::set<Document>& all() noexcept;
        unsigned documentCount() const noexcept;

    private:
        DocumentIndex dIdx;
        DocumentStore dStore;
        std::map<std::string, std::set<unsigned>> termToDocuments;
        std::set<Document> foundDocuments;
    };
}

#endif