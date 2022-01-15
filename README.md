# Simple-search-engine

## Instructions

Clone repository with 

```
git clone https://github.com/MrPekar98/Simple-Search-Engine.git
```

### Prerequisites

Install Curl

```
sudo apt install curl -y && sudo apt install libcurl4-gnutls-dev
```

Install Microsoft C++ REST SDK

```
sudo apt-get install libcpprest-dev
```

To install on a platform different from a Debian-based platform, take a look at the _Getting Started_ section <a href="https://github.com/microsoft/cpprestsdk/blob/master/README.md">here</a>.

Run the following command to install necessary build tools

```
sudo apt-get install g++ make cmake git libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev libwebsocketpp-dev openssl libssl-dev ninja-build
```

Clone the respository

```
git clone https://github.com/Microsoft/cpprestsdk.git casablanca
```

Run the following set of commands (you can specify `-DCMAKE_BUILD_TYPE=Release` instead to build a release version)

```
cd casablanca
mkdir build.debug
cd build.debug
cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Debug
ninja
sudo ninja install
```

### Compile

To compile the project, simply run the command

```
make
```

Now, an executable _search_ is built in the project root.

Note that you can specify the crawler seed set in _src/config.hpp_. Choosing a set of web pages with a high out-degree is recommended.

### Docker

Alternatively, build the Docker image

```
docker build -t search .
```

Run the Docker container

```
docker run --name search -p <PORT>:<PORT> search
```

Set _\<PORT\>_ to the port number specified in _src/config.hpp_.
Add the flag  _-d_ to detach from the process.

## Communicating With the Search Engine

A simple web page is provided with a search bar. Otherwise, you can send a simple POST request with search keyword in the request body.
The search result is a simple plain text with relavant document titles and their URL.