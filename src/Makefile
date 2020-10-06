CC=g++
SRC=main.cpp
IN := crawler/crawler.cpp crawler/data/frontier.cpp crawler/data/document.cpp crawler/shingles.cpp crawler/data/postings_list.cpp indexing/term.cpp pagerank/pagerank.cpp lib/html_parser/src/cppw_htmlparser.cpp lib/html_parser/src/cppw_string.cpp
INCLUDE := lib/html_parser/src/ lib/hash
CFLAGS=-lcurl -pthread $(foreach h, $(INCLUDE), -I $(h))

scam: $(SRC)
	$(CC) -o scam $(SRC) $(foreach i, $(IN), $(i)) $(CFLAGS)

test:
	$(CC) -o frontier_test tests/frontier_test.cpp crawler/data/frontier.cpp
	$(CC) -o postings_list_test tests/postings_list_test.cpp crawler/data/postings_list.cpp indexing/term.cpp
	$(CC) -o term_test tests/term_test.cpp indexing/term.cpp
	./frontier_test
	./postings_list_test
	./term_test

clean:
	rm scam frontier_test postings_list_test term_test