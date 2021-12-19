#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <utility>
#include <sstream>
#include "../serializable.hpp"

namespace Pekar
{
    struct Block: public Serializable
    {
        long pos, blockLength;

        template<typename Long1, typename Long2>
        Block(Long1&& pos, Long2&& blockLength)
            : pos(std::forward<Long1>(pos)), blockLength(std::forward<Long2>(blockLength))
        {}

        Block()
            : pos(0), blockLength(0)
        {}

        Block(const Block& other) = default;
        Block(Block&& other) = default;
        Block& operator=(const Block& other) = default;
        Block& operator=(Block&& other) = default;

        std::string serialize() const override
        {
            return std::to_string(this->pos) + "###" + std::to_string(this->blockLength) + "###";
        }

        static Block deserialize(const std::string& ser)
        {
            unsigned i = 0;
            char c;
            std::string pos = "", length = "";

            while ((c = ser[i++]) != '#' || ser[i] != '#' || ser[i + 1] != '#')
            {
                pos += c;
            }

            i += 2;

            while ((c = ser[i++]) != '#' || ser[i] != '#' || ser[i + 1] != '#')
            {
                length += c;
            }

            return Block(std::stol(pos), std::stol(length));
        }
    };

    class Location: public Serializable
    {
    public:
        template<typename Str, typename FileBlock>
        Location(Str&& file, FileBlock&& fileBlock)
            : file(std::forward<Str>(file)), fileBlock(std::forward<Block>(fileBlock))
        {}

        Location()
            : file("")
        {}

        Location(const Location& other) = default;
        Location(Location&& other) = default;
        Location& operator=(const Location& other) = default;
        Location& operator=(Location&& other) = default;

        const std::string& getFileName() const noexcept
        {
            return this->file;
        }

        const Block& getFileBlock() const noexcept
        {
            return this->fileBlock;
        }

        std::string serialize() const override
        {
            return this->file + "###" + this->fileBlock.serialize();
        }

        static Location deserialize(const std::string& ser)
        {
            std::string file = "";
            unsigned i = 0;
            char c;

            while ((c = ser[i++]) != '#' || ser[i] != '#' || ser[i + 1] != '#')
            {
                file += c;
            }

            i += 2;

            return Location(file, Block::deserialize(ser.substr(i)));
        }

        bool operator<(const Location& other) const noexcept
        {
            return this->fileBlock.pos < other.getFileBlock().pos;
        }

    private:
        std::string file;
        Block fileBlock;
    };
}

#endif