# Simple Database Server

## Introduction

This project is a simple database server that communicates with clients using the Protocol Buffers (protobuf) format. It demonstrates basic client-server interaction, data serialization, and communication using protobuf.

## Prerequisites

Before building and running the application, make sure to install the following dependencies:

- **zlog**: A reliable, pure C logging library.
- **protobuf-c**: A C implementation of Google's Protocol Buffers.

Please follow the installation instructions provided by these libraries.

## Building the Application

To build the server and client applications, clone the repository and use `make`:

```
git clone https://github.com/LeTrongDat/itmo
cd itmo/low-level-programming/lab3
make all
```

This will compile both the server and client applications and place the executables in the `build` directory.

## Running the Application

### Starting the Server

Navigate to the `build` directory and start the server:

```
cd build
./server 8000 test
```

This starts the server listening on port `8000` and uses `test` as the database name.

### Running the Client

The client can be run in two modes: interactive mode and script mode.

#### Interactive Mode

For interactive mode, where commands are entered via `stdin`, run:

```
./client 8000
```

This allows you to manually enter commands that will be sent to the server.

#### Script Mode

To run the client using a predefined set of commands from a file, use:

```
./client 8000 ../tests/seed.sql
```

This will execute the commands listed in `seed.sql` file.