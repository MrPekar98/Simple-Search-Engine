#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "config.hpp"
#include <system_error>

void parse()
{
    std::error_code err = std::make_error_code(std::errc::protocol_error);

#ifndef DATA_PATH
    throw std::system_error(err, "Mising data path");
#endif

#ifndef DATA_FILE
    throw std::system_error(err, "Missing data file name");
#endif

#ifndef STOP_WORDS
    throw std::system_error(err, "Missing stop words file");
#endif
}

#endif