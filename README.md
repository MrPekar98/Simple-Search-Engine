## Simple-search-engine

Clone repository with 

`git clone --recursive https://github.com/MrPekar98/Simple-Search-Engine.git`

### Prerequisites

Install Curl

```sudo apt install curl -y && sudo apt install libcurl4-gnutls-dev```

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

## Compile

To compile the project, simply run the command

```
make
```

Now, an executable _search_ is built in the project root.

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