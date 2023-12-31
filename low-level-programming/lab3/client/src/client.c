#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../../proto/database.pb-c.h"

#define BUFFER_SIZE 1024

#define RED   "\x1B[31m"
#define GREEN "\x1B[32m"
#define RESET "\x1B[0m"

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <port> [seed]\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    FILE *input_stream = argc == 3 ? fopen(argv[2], "r") : stdin;

    if (port <= 0) {
        fprintf(stderr, "Invalid port number.\n");
        return 1;
    }

    if (argc == 3 && !input_stream) {
        perror("Failed to open seed file");
        return 1;
    }

    int sock;
    struct sockaddr_in serv_addr;
    char input[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Socket creation error\n");
        if (argc == 3) fclose(input_stream);
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        if (argc == 3) fclose(input_stream);
        return -1;
    }

    while (1) {
        if (!fgets(input, BUFFER_SIZE, input_stream)) {
            if (feof(input_stream)) break;
            printf("Error reading input\n");
            break;
        }
        input[strcspn(input, "\n")] = 0; // Remove newline character

        if (strcmp(input, "exit") == 0) {
            break;
        }

        if (input[0] == '\0') continue;

        printf("> ");

        if (input_stream != stdin) {
            printf(RED "Executing command: %s\n" RESET, input);
        } else {
            printf(RED "Enter query to send (or 'exit' to quit): " RESET);
        }

        Database__Request request = DATABASE__REQUEST__INIT;
        void *buf;
        unsigned len;

        request.query = input;
        len = database__request__get_packed_size(&request);
        buf = malloc(len);
        database__request__pack(&request, buf);

        send(sock, buf, len, 0);
        free(buf);

        int bytesReceived = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived < 1) {
            printf("Connection closed by server\n");
            break;
        }

        Database__Response *response;
        response = database__response__unpack(NULL, bytesReceived, (uint8_t *)buffer);
        if (response != NULL) {
            printf(GREEN "> Response from server: %s\n" RESET, response->message);
            database__response__free_unpacked(response, NULL);
        } else {
            printf("Failed to unpack response\n");
        }
    }

    close(sock);
    if (argc == 3) fclose(input_stream);

    return 0;
}
