CC=g++
SRC=main.cpp
IN := crawler/crawler.cpp crawler/data/frontier.cpp crawler/data/document.cpp crawler/shingles.cpp crawler/data/postings_list.cpp lib/html_parser/src/cppw_htmlparser.cpp lib/html_parser/src/cppw_string.cpp
INCLUDE := lib/html_parser/src/ lib/hash
CFLAGS=-lcurl -pthread $(foreach h, $(INCLUDE), -I $(h))

scam: $(SRC)
	$(CC) -o scam $(SRC) $(foreach i, $(IN), $(i)) $(CFLAGS)

clean:
	rm scam
