# Define compiler and flags
CC := gcc
CFLAGS := -I/usr/local/include -I$(SERVER_DIR)/include
LDFLAGS := -L/usr/local/lib -lprotobuf-c

# Define paths
PROTO_DIR := proto
SERVER_DIR := server
BUILD_DIR := $(SERVER_DIR)/build

# Find .proto files
PROTO_FILES := $(wildcard $(PROTO_DIR)/*.proto)

# Define targets for .c and .h files
PB_C_FILES := $(patsubst $(PROTO_DIR)/%.proto,$(BUILD_DIR)/%.pb-c.c,$(PROTO_FILES))
PB_H_FILES := $(patsubst $(PROTO_DIR)/%.proto,$(BUILD_DIR)/%.pb-c.h,$(PROTO_FILES))

# Server source files
SERVER_SOURCES := $(wildcard $(SERVER_DIR)/src/*.c) $(wildcard $(SERVER_DIR)/*.c)
SERVER_OBJECTS := $(SERVER_SOURCES:.c=.o) $(PB_C_FILES:.c=.o)

# Default target
all: server

# Generate .pb-c.c and .pb-c.h files from .proto files
$(BUILD_DIR)/%.pb-c.c $(BUILD_DIR)/%.pb-c.h: $(PROTO_DIR)/%.proto
	@mkdir -p $(BUILD_DIR)
	protoc-c --c_out=$(BUILD_DIR) --proto_path=$(PROTO_DIR) $<

# Compile server
server: $(SERVER_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compile server objects
$(SERVER_DIR)/%.o: $(SERVER_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR)/*
	rm -f $(SERVER_DIR)/*.o
	rm -f server

.PHONY: all server clean

