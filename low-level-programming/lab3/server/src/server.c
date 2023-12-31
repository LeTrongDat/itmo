#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "zlog.h"
#include "../../proto/database.pb-c.h"
#include "../include/request_handler.h"

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <database>\n", argv[0]);
        return -1;
    }

    int PORT = atoi(argv[1]);
    const char *database_name = argv[2];

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    zlog_category_t *c;

    if (zlog_init(NULL)) {
        fprintf(stderr, "zlog initialization failed\n");
        return -1;
    }

    c = zlog_get_category("my_category");
    if (!c) {
        fprintf(stderr, "zlog get category failed\n");
        zlog_fini();
        return -2;
    }
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        zlog_error(c, "Socket creation failed");
        zlog_fini();
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        zlog_error(c, "Bind failed");
        zlog_fini();
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        zlog_error(c, "Listen failed");
        zlog_fini();
        return -1;
    }

    Database *db = openDatabase(database_name);
    if (db == NULL) {
        db = createDatabase(database_name);
        if (db == NULL) {
            zlog_error(c, "Failed to open or create database");
            zlog_fini();
            return -1;
        }
    }

    zlog_info(c, "Database opened or created successfully");

    zlog_info(c, "Server started on port %d", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            zlog_error(c, "Accept failed");
            continue;
        }

        while (1) {
            uint8_t buffer[MAX_BUFFER_SIZE];
            int bytes_received = recv(new_socket, buffer, MAX_BUFFER_SIZE, 0);
            if (bytes_received < 0) {
                zlog_error(c, "Failed to receive message");
                close(new_socket);
                break;
            }

            Database__Request *request;
            request = database__request__unpack(NULL, bytes_received, buffer);
            if (request == NULL) {
                zlog_error(c, "Failed to unpack request");
                close(new_socket);
                break;
            }

            if (strcmp(request->query, "") == 0) continue;

            zlog_info(c, "Receive request from client: %s", request->query);

            Database__Response response = DATABASE__RESPONSE__INIT;
            handleRequest(request, &response, db); 
            database__request__free_unpacked(request, NULL);

            unsigned response_length = database__response__get_packed_size(&response);
            void *response_buffer = malloc(response_length);
            database__response__pack(&response, response_buffer);

            send(new_socket, response_buffer, response_length, 0);

            free(response_buffer);
        }
        close(new_socket);
    }

    closeDatabase(db);
    zlog_fini();
    close(server_fd);
    return 0;
}
