FROM ubuntu

RUN apt update
RUN apt install curl libcurl4-gnutls-dev -y
RUN apt-get install libcpprest-dev -y
RUN apt-get install g++ make cmake git -y
RUN apt-get install libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev -y
RUN apt-get install libwebsocketpp-dev -y
RUN apt-get install openssl libssl-dev -y
RUN apt-get install ninja-build -y

RUN git clone https://github.com/MrPekar98/Simple-Search-Engine.git search
WORKDIR search/

RUN git clone https://github.com/Microsoft/cpprestsdk.git casablanca
RUN cd casablanca && mkdir build.debug && cd build.debug && cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Release && ninja && ninja install && cd /search
RUN make

CMD [ "./search" ]