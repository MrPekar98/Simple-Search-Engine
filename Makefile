CC=g++
BASE=src
SRC := search.cpp index/document_index.cpp store/document_store.cpp query/query.cpp index/postings_list.cpp query/ranker.cpp query/document_search.cpp crawler/crawler.cpp crawler/frontier.cpp
LIBS := lib/similarity/shingle.cpp
INCLUDE=include
CFLAGS=$(foreach l, $(LIBS), $(l)) -I$(INCLUDE) -pthread -lcurl -I deps/html_parser/src deps/html_parser/src/cppw_htmlparser.cpp deps/html_parser/src/cppw_string.cpp

search: ; \
	$(CC) -o search $(foreach s, $(SRC), $(BASE)/$(s)) $(CFLAGS)

clean: ; \
	rm search