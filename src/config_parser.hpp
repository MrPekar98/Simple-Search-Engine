#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "config.hpp"
#include <system_error>

void parse()
{
    std::error_code err = std::make_error_code(std::errc::protocol_error);

#ifndef DATA_FILE
    throw std::system_error(err, "Missing data file name");
#endif

#ifndef STOP_WORDS
    throw std::system_error(err, "Missing stop words file");
#endif

#ifndef SIM_THRES
    throw std::system_error(err, "Missing similarity threshold");
#endif

#ifndef THREADS
    throw std::system_error(err, "Missing number of threads");
#endif

#ifndef STORE_CONTENT
    throw std::system_error(err, "Missing whether to store document content");
#endif
}

#endif