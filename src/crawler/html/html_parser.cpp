#include "html_parser.hpp"
#include <utility>

namespace Pekar
{
    BaseHtmlParser::BaseHtmlParser(const std::string& html)
        : html(html)
    {}

    BaseHtmlParser::BaseHtmlParser(std::string&& html)
        : html(std::move(html))
    {}

    std::string BaseHtmlParser::title() const
    {
        size_t titleStartPos = this->html.find("<title");
        titleStartPos = this->html.find(">", titleStartPos);
        titleStartPos++;

        size_t titleEndPos = this->html.find("</title>", titleStartPos);
        return this->html.substr(titleStartPos, titleEndPos - titleStartPos);
    }

    std::string BaseHtmlParser::body() const
    {
        size_t bodyStartPos = this->html.find("<body");
        bodyStartPos = this->html.find(">", bodyStartPos);
        bodyStartPos++;

        size_t bodyEndPos = this->html.find("</body>", bodyStartPos);
        return this->html.substr(bodyStartPos, bodyEndPos - bodyStartPos);
    }

    std::vector<std::string> BaseHtmlParser::paragraphs() const
    {
        size_t paragraphStart = 0, paragraphEnd = 0;
        std::vector<std::string> pv;

        while (paragraphEnd != std::string::npos)
        {
            paragraphStart = this->html.find("<p", paragraphEnd);

            if (paragraphStart == std::string::npos)
                break;

            paragraphStart = this->html.find(">", paragraphStart);
            paragraphStart++;

            paragraphEnd = this->html.find("</p>", paragraphStart);
            pv.push_back(this->html.substr(paragraphStart, paragraphEnd - paragraphStart));
        }

        return pv;
    }

    std::set<std::string> BaseHtmlParser::links() const noexcept
    {
        std::set<std::string> links;
        size_t start = -1, len = 0;

        do
        {
            start = this->html.find("http", start + 1);

            if (start == std::string::npos)
                break;

            len = BaseHtmlParser::urlStringEndIdx(this->html.substr(start));

            if (len == -1)
                continue;

            links.insert(this->html.substr(start, len));
        }
        while (start != std::string::npos);

        return links;
    }

    int BaseHtmlParser::urlStringEndIdx(const std::string& str) noexcept
    {
        unsigned idx = str.find("://");
        idx += 3;
        unsigned LENGTH_LIMIT = 50;

        for (; idx < str.size(); idx++)
        {
            if (idx >= LENGTH_LIMIT)
                return -1;

            switch (str[idx])
            {
                case '"':
                case '\'':
                case ' ':
                case ':':
                case ';':
                    return idx;
                case '\n':
                case '\t':
                    return -1;
            }
        }

        return idx;
    }
}