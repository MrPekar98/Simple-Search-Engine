#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include <string>
#include <set>
#include <utility>
#include <type_traits>
#include <sstream>
#include <algorithm>
#include "serializable.hpp"

namespace Pekar
{
    static unsigned idCounter = 1;

    class Document: public Serializable
    {
    public:
        using Iterator = typename std::set<std::string>::iterator;
        using ConstIterator = typename std::set<std::string>::const_iterator;

        template<typename Url, typename Content, typename OutLinks>
        Document(Url&& url, Content&& content, OutLinks&& outLinks)
            : url(std::forward<Url>(url)), content(std::forward<Content>(content)), outLinks(std::forward<OutLinks>(outLinks))
        {
            this->id = idCounter++;
        }

        Document() = delete;
        Document(const Document& other) = default;
        Document(Document&& other) = default;
        Document& operator=(const Document& other) = default;
        Document& operator=(Document&& other) = default;

        // This setter can be dangerous to call!
        template<typename Id>
        void setId(Id&& id)
        {
            this->id = std::forward<Id>(id);
        }

        const unsigned& getId() const noexcept
        {
            return this->id;
        }

        const std::string& getUrl() const noexcept
        {
            return this->url;
        }

        const std::string& getContent() const noexcept
        {
            return this->content;
        }

        // Iterator of out links
        Iterator begin()
        {
            return this->outLinks.begin();
        }

        Iterator end()
        {
            return this->outLinks.end();
        }

        ConstIterator cbegin()
        {
            return this->outLinks.cbegin();
        }

        ConstIterator cend()
        {
            return this->outLinks.cend();
        }

        bool operator<(const Document& other) const noexcept
        {
            return this->id < other.getId();
        }

        bool operator==(const Document& other) const noexcept
        {
            if (this->id != other.getId() || this->url != other.getUrl() || this->content != other.getContent())
                return false;

            for (auto it1 = this->outLinks.cbegin(), it2 = other.outLinks.cbegin(); 
                    it1 != this->outLinks.cend() && it2 != other.outLinks.cend(); it1++, it2++)
            {
                if (*it1 != *it2)
                    return false;
            }

            return true;
        }

        std::string serialize() const noexcept override
        {
            std::string ser;
            std::stringstream ss;
            ss << this->id;
            ss >> ser;
            ser += "###" + this->url + "###" + this->content + "###";

            for (auto it = this->outLinks.cbegin(); it != this->outLinks.cend(); it++)
            {
                ser += *it + ",";
            }

            if (this->outLinks.size() > 0)
            {
                ser[ser.size() - 1] = '#';
                ser += "##";
            }

            else
                ser += "###";

            return ser;
        }

        static Document deserialize(const std::string ser)
        {
            if (std::count(ser.cbegin(), ser.cend(), '#') != 12)
                throw "Serialized document parsing error!";

            unsigned i = 0;
            char c;
            std::string id = "", url = "", content = "", outLink = "";
            std::set<std::string> outLinks;

            while ((c = ser[i++]) != '#' || ser[i] != '#' || ser[i + 1] != '#')
            {
                id += c;
            }

            i += 2;

            while ((c = ser[i++]) != '#' || ser[i] != '#' || ser[i + 1] != '#')
            {
                url += c;
            }

            i += 2;

            while ((c = ser[i++]) != '#' || ser[i] != '#' || ser[i + 1] != '#')
            {
                content += c;
            }

            i += 2;
            
            while ((c = ser[i++]) != '#' || ser[i] != '#' || ser[i + 1] != '#')
            {
                if (c == ',')
                {
                    outLinks.insert(outLink);
                    outLink = "";
                    continue;
                }

                outLink += c;
            }

            outLinks.insert(outLink);

            Document d(url, content, outLinks);
            d.setId(std::stoi(id));
            return d;
        }

    private:
        std::string url, content;
        std::set<std::string> outLinks;
        unsigned id;
    };
}

#endif