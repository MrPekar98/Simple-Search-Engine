#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP

#include <string>

namespace Pekar
{
    class Serializable
    {
    public:
        virtual ~Serializable() {}
        virtual std::string serialize() const = 0;
    };
}

#endif