CC=g++
SRC=main.cpp
IN := crawler.cpp data/frontier.cpp
CFLAGS=-lcurl -pthread

scam: $(SRC)
	$(CC) -o scam $(SRC) $(foreach i, $(IN), $(i)) $(CFLAGS)

clean:
	rm scam
