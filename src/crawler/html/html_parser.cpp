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
}