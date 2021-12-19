CC=g++
BASE=src
SRC := search.cpp
LIBS := lib/similarity/shingle.cpp
INCLUDE=include
CFLAGS=$(foreach l, $(LIBS), $(l)) -I$(INCLUDE) -I deps/html_parser/src -L deps/html_parser/src -I deps/curl/include -L deps/curl/lib

search: ; \
	$(CC) -o search $(foreach s, $(SRC), $(BASE)/$(s)) $(CFLAGS)

clean: ; \
	rm search