#include "render_page.hpp"
#include <fstream>

namespace Pekar
{
    std::string Render::render(const std::string& file)
    {
        std::ifstream stream(file);
        char c;
        std::string rendered = "";

        if (!stream)
            return "";

        while (stream.get(c))
        {
            rendered += c;
        }

        return rendered;
    }
}