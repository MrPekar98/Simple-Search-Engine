CC=g++
SRC=main.cpp
IN := crawler.cpp data/frontier.cpp shingles.cpp
CFLAGS=-lcurl

scam: $(SRC)
	$(CC) -o scam $(SRC) $(foreach i, $(IN), $(i)) $(CFLAGS)

clean:
	rm scam
