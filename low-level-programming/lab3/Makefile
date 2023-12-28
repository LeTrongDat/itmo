CC=gcc
CFLAGS=-I /opt/homebrew/include -L /opt/homebrew/lib
DEPS = message.pb-c.h database.pb-c.h
LIBS=-lprotobuf-c -lzlog

all: gen_proto server client

gen_proto:
	protoc --c_out=. database.proto

server: server.c database.pb-c.c
	$(CC) $(CFLAGS) -o server server.c database.pb-c.c request_handler.c $(LIBS)

client: client.c database.pb-c.c
	$(CC) $(CFLAGS) -o client client.c database.pb-c.c request_handler.c $(LIBS)

clean:
	rm -f *.o server client database.pb-c.h database.pb-c.c
