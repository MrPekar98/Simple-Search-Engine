#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <set>
#include <string>

// Data store file
#define DATA_FILE "data/data"

// Stop words file
#define STOP_WORDS "assets/stop_words.txt"

// Searching
#define SIM_THRES 0.5
#define MIN_RANK_SCORE 0.5
#define THREADS 1
#define STORE_CONTENT 0
#define SHINGLES 3
#define SEED std::set<std::string>({"https://www.apple.com/", "https://www.microsoft.com/"})

// Server
#define HOST "127.0.0.1localhost"
#define PORT 8080

#endif