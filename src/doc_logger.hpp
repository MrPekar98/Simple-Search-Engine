#ifndef DOC_LOGGER_H
#define DOC_LOGGER_H
#include <vector>
#include <string>
#include <iostream>
#include "crawler/data/document.hpp"

namespace scam
{
    class log
    {
    public:
        static void log_all(const std::vector<scam::crawler::document>& docs, std::ostream& out) noexcept
        {
            for (unsigned i = 0; i < docs.size(); i++)
            {
                log::log_one(docs[i], out);
            }
        }

        static void log_one(const scam::crawler::document& doc, std::ostream& out) noexcept
        {
            out << doc.url << " -> " << doc.content << std::endl;
        }
    };
}

#endif
