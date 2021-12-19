#include "document_store.hpp"

namespace Pekar
{
    DocumentStore::DocumentStore(const std::string& dataFile)
        : file(dataFile), tempDocument(Document("", "", std::set<std::string>()))
    {
        std::ifstream input(dataFile);

        if (input)
            loadFromFile(input);

        else
            std::fstream output(dataFile, std::ios_base::out);
    }

    std::set<Location> DocumentStore::load(const std::set<Document>& items)
    {
        std::set<Location> lSet;

        for (auto it = items.cbegin(); it != items.cend(); it++)
        {
            Location l = add(*it);
            lSet.insert(l);
            this->locations.insert(l);
        }

        return lSet;
    }

    Location DocumentStore::add(const Document& item)
    {
        this->mtx.lock();

        std::string serialized = item.serialize();
        std::fstream out(this->file, std::ios_base::app);
        checkFile(out);

        long pos = out.tellp();
        out << serialized;

        Location l(this->file, Block((long) pos, (long) serialized.size()));
        this->locations.insert(l);
        this->mtx.unlock();

        return l;
    }

    // This cannot be support yet
    // Requires updating location of all sequentially following documents
    void DocumentStore::remove(const Location& key)
    {
        throw "Unsupported operation";
    }

    Document& DocumentStore::find(const Location& key)
    {
        this->mtx.lock();

        if (this->locations.find(key) == this->locations.end())
        {
            this->mtx.unlock();
            throw "Nothing at given location";
        }

        std::fstream in(this->file, std::ios_base::in);
        checkFile(in);
        in.seekg(key.getFileBlock().pos);

        char* buffer = new char[key.getFileBlock().blockLength];
        in.read(buffer, key.getFileBlock().blockLength);
        this->tempDocument = Document::deserialize(std::string(buffer));

        this->mtx.unlock();
        return this->tempDocument;
    }

    // This is unfortunately dependent on how Document is serialized!
    void DocumentStore::loadFromFile(std::ifstream& file)
    {
        this->mtx.lock();

        std::string serialized = "";
        char c;
        unsigned counter = 0;
        long pos = 0;

        while (file.get(c))
        {
            serialized += c;

            if (c == '#')
            {
                counter++;

                if (counter == 12)  // 12 is the number char '#' per object
                {
                    counter = 0;
                    this->locations.insert(Location(this->file, Block(pos, serialized.size())));

                    pos = (long) file.tellg();
                    serialized = "";
                }
            }
        }

        this->mtx.unlock();
    }

    const std::set<Location>& DocumentStore::getLocations() const noexcept
    {
        return this->locations;
    }

    // TODO: This should be a macro
    void DocumentStore::checkFile(const std::fstream& stream) const
    {
        if (!stream)
            throw "Failed to open document data file";
    }
}