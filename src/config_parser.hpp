#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "config.hpp"
#include <system_error>

void parse()
{
    std::error_code err = std::make_error_code(std::errc::protocol_error);

#ifndef DATA_FILE
    throw std::system_error(err, "Missing data file name");
#elif !defined(STOP_WORDS)
    throw std::system_error(err, "Missing stop words file");
#elif !defined(SIM_THRES)
    throw std::system_error(err, "Missing similarity threshold");
#elif !defined(THREADS)
    throw std::system_error(err, "Missing number of threads");
#elif !defined(STORE_CONTENT)
    throw std::system_error(err, "Missing whether to store document content");
#elif !defined(MIN_RANK_SCORE)
    throw std::system_error(err, "Missing min rank score");
#elif !defined(SHINGLES)
    throw std::system_error(err, "Shingle length not defined");
#elif !defined(HOST)
    throw std::system_error(err, "Missing host address");
#elif !defined(PORT)
    throw std::system_error(err, "Missing port number specification");
#elif !defined(HOMEPAGE)
    throw std::system_error(err, "Missing homepage HTML directory path");
#endif
}

#endif