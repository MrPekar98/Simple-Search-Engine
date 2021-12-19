#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <string>
#include <type_traits>
#include <utility>

namespace Pekar
{
    // Only numeric values and strings for now
    class Serializer
    {
    public:
        template <typename T>
        static std::string serialize(T&& t)
        {
            // For some reason, std::string is true here
            if (std::is_integral<typename std::remove_reference<T>::type>::value || std::is_floating_point<typename std::remove_reference<T>::type>::value)
                return std::to_string(std::forward<T>(t));

            return std::forward<T>(t) + "###";
        }

        template<typename T>
        static T deserialize(const std::string& ser)
        {
            std::string val = "";
            unsigned i = 0;
            char c;

            while ((c = ser[i++]) != '#' || ser[i] != '#' || ser[i] != '#')
            {
                val += c;
            }

            if (std::is_integral<T>::value)
                return std::stoi(val);

            else if (std::is_floating_point<typename std::remove_reference<T>::type>::value)
                return std::stod(val);

            return val;
        }
    };
}

#endif