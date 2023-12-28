.PHONY: build run

BUILDDIR = build
SVDIR = server
CLDIR = client
INCDIR = include
TESTDIR = tests
PROTODIR = proto

CC=gcc
CFLAGS=-L /opt/homebrew/lib -I /opt/homebrew/include -I ../lab1/include
LIBS=-lprotobuf-c -lzlog
DATABASE_CORE=../lab1/src/*.c
PARSER_CORE=../lab2/build/*.c ../lab2/src/types.c

build: 
	@mkdir -p $(BUILDDIR)

gen_proto: build
	cd proto && protoc --c_out=. database.proto && cd ..

gen_parser: build
	cd ../lab2 && make build/myparser && cd ../lab3

server: build gen_proto gen_parser
	$(CC) $(CFLAGS) -o $(BUILDDIR)/server $(SVDIR)/**/*.c $(PROTODIR)/*.c $(DATABASE_CORE) $(PARSER_CORE) $(LIBS)

client: build
	$(CC) $(CFLAGS) -o $(BUILDDIR)/client $(CLDIR)/**/*.c $(PROTODIR)/*.c $(LIBS)

# run:
# 	cd build && ./server 8000 db & 
# 	cd build && ./client

clean:
	rm -f *.o server client database.pb-c.h database.pb-c.c

all: gen_proto gen_parser build server client 

