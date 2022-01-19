#ifndef RENDER_PAGE_HPP
#define RENDER_PAGE_HPP

#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <stdexcept>

namespace Pekar
{
    class Render
    {
    public:
        template<typename File, typename ... Args>
        static std::string render(File&& file, Args&&... args)
        {
            std::out_of_range exception("Number of arguments does not match number of placeholder in rendered content");
            std::string content = Render::loadFile(std::forward<std::string>(file));
            std::vector<std::string> argsVec({std::forward<std::string>(args)...});
            unsigned argPointer = 0;

            for (unsigned i = 0; i < content.length(); i++)
            {
                if (i < content.length() - 2 && content[i] == '{' && content[i + 1] == '#' && content[i + 2] == '}')
                {
                    if (argPointer >= argsVec.size())
                        throw exception;

                    content.replace(i, 3, argsVec[argPointer++]);
                }
            }

            if (argPointer < argsVec.size())
                throw exception;

            return content;
        }

    private:
        template<typename File>
        static std::string loadFile(File&& file)
        {
            std::ifstream stream(std::forward<std::string>(file));
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
    };
}

#endif