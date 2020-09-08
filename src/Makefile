CC=g++
SRC=main.cpp
IN := crawler.cpp data/frontier.cpp shingles.cpp lib/html_parser/src/cppw_htmlparser.cpp lib/html_parser/src/cppw_string.cpp
INCLUDE := lib/html_parser/
CFLAGS=-lcurl -pthread $(foreach h, $(INCLUDE), -I $(h))

scam: $(SRC)
	$(CC) -o scam $(SRC) $(foreach i, $(IN), $(i)) $(CFLAGS)

clean:
	rm scam
