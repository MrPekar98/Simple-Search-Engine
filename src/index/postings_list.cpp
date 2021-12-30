#include "postings_list.hpp"
#include "../tokenizer.hpp"
#include <utility>

namespace Pekar
{
    PostingsList::PostingsList(const std::string& dataFile)
        : dStore(dataFile)
    {
        std::set<Location> locations = this->dStore.getLocations();
        std::set<Document> documents;

        for (auto it = locations.cbegin(); it != locations.cend(); it++)
        {
            documents.insert(this->dStore.find(*it));
        }

        if (documents.size() > 0)
            add(documents);
    }

    std::set<std::string> PostingsList::load(const std::set<std::set<Document>>& items)
    {
        std::set<std::string> terms;

        for (const std::set<Document>& docs : items)
        {
            terms.insert(add(docs));
        }

        return terms;
    }

    // Makes no sense to return a single string, but it's the required key
    std::string PostingsList::add(const std::set<Document>& item)
    {
        for (const Document& d : item)
        {
            std::vector<Term> terms = Tokenizer::tokenize(d.getContent());
            Location l = this->dStore.add(d);   // Here
            unsigned id = this->dIdx.add(l);

            for (const Term& t : terms)
            {
                if (this->termToDocuments.find(t.getTerm()) == this->termToDocuments.end())
                    this->termToDocuments[t.getTerm()] = std::set<unsigned>();

                this->termToDocuments[t.getTerm()].insert(id);
            }
        }

        return "";
    }

    // Nothing is removed from document index and document store because they can exist in other entries
    void PostingsList::remove(const std::string& key)
    {
        this->termToDocuments.erase(key);
    }

    std::set<Document>& PostingsList::find(const std::string& key)
    {
        std::set<unsigned> locationsIds = this->termToDocuments[key];
        this->foundDocuments.clear();

        for (const unsigned& lId : locationsIds)
        {
            Location l = this->dIdx.find(lId);
            this->foundDocuments.insert(this->dStore.find(std::move(l)));
        }

        return this->foundDocuments;
    }
}