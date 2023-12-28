#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "database.pb-c.h"

#define PORT 8080
#define SERVER_ADDR "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char input[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, SERVER_ADDR, &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    while (1) {
        printf("Enter query to send (or 'exit' to quit): ");
        fgets(input, BUFFER_SIZE, stdin);
        input[strcspn(input, "\n")] = 0; // Remove newline character

        if (strcmp(input, "exit") == 0) {
            break;
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
            printf("Response from server: %s\n", response->message);
            database__response__free_unpacked(response, NULL);
        } else {
            printf("Failed to unpack response\n");
        }
    }

    close(sock);

    return 0;
}
