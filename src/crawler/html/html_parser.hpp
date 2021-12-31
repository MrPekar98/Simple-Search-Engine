#ifndef HTML_PARSER_HPP
#define HTML_PARSER_HPP

#include <string>
#include <vector>
#include <set>

namespace Pekar
{
    // TODO: This should be extended
    class HtmlParser
    {
    public:
        virtual ~HtmlParser() {}
        virtual std::string title() const = 0;
        virtual std::string body() const = 0;
        virtual std::vector<std::string> paragraphs() const = 0;
    };

    class BaseHtmlParser: public HtmlParser
    {
    public:
        BaseHtmlParser() = delete;
        BaseHtmlParser(const std::string& html);
        BaseHtmlParser(std::string&& html);
        std::string title() const override;
        std::string body() const override;
        std::vector<std::string> paragraphs() const override;
        std::set<std::string> links() const noexcept;

    private:
        static int urlStringEndIdx(const std::string& str) noexcept;
        std::string html;
    };
}

#endif