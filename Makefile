CC=g++
BASE=src
SRC := search.cpp index/document_index.cpp store/document_store.cpp query/query.cpp index/postings_list.cpp query/ranker.cpp query/document_search.cpp crawler/crawler.cpp crawler/frontier.cpp
LIBS := lib/similarity/shingle.cpp
INCLUDE=include
CFLAGS=$(foreach l, $(LIBS), $(l)) -I$(INCLUDE) -I deps/html_parser/src -L deps/html_parser/src -I deps/curl/include -L deps/curl/lib

search: ; \
	$(CC) -o search $(foreach s, $(SRC), $(BASE)/$(s)) $(CFLAGS)

clean: ; \
	rm search