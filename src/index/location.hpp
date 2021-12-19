#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <utility>

namespace Pekar
{
    struct Block
    {
        unsigned row, column, blockLength;

        template<typename Unsigned>
        Block(Unsigned&& row, Unsigned&& column, Unsigned&& blockLength)
            : row(std::forward<Unsigned>(row)), column(std::forward<Unsigned>(column)), blockLength(std::forward<Unsigned>(blockLength))
        {}
    };

    class Location
    {
    public:
        template<typename Str, typename FileBlock>
        Location(Str&& file, FileBlock&& fileBlock)
            : file(std::forward<Str>(file)), fileBlock(std::forward<Block>(fileBlock))
        {}

        const std::string& getFileName() const noexcept
        {
            return this->file;
        }

        const Block& getFileBlock() const noexcept
        {
            return this->fileBlock;
        }

    private:
        std::string file;
        Block fileBlock;
    };
}

#endif