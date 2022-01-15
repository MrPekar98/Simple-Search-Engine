CC=g++
BASE=src
SRC := search.cpp index/document_index.cpp store/document_store.cpp query/query.cpp index/postings_list.cpp query/ranker.cpp query/document_search.cpp crawler/crawler.cpp crawler/frontier.cpp crawler/html/html_parser.cpp server/server.cpp server/render_page.cpp
LIBS := lib/similarity/shingle.cpp
INCLUDE=include
CFLAGS=$(foreach l, $(LIBS), $(l)) -I$(INCLUDE) -pthread -lcurl -lboost_system -lcrypto -lssl -lcpprest

search: ; \
	$(CC) -o search $(foreach s, $(SRC), $(BASE)/$(s)) $(CFLAGS)

clean: ; \
	rm search