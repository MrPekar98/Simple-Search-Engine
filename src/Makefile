CC=g++
SRC=main.cpp
IN := crawler.cpp
CFLAGS=-lcurl -pthread

scam: $(SRC)
	$(CC) -o scam $(SRC) $(foreach i, $(IN), $(i)) $(CFLAGS)

clean:
	rm scam
