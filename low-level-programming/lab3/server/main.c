#include <sys/socket.h>
#include <netinet/in.h>
#include "build/proto/db.pb-c.h"

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  // Creating socket file descriptor
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Forcefully attaching socket to the port 8080
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(8080);

  // Bind the socket to the address
  bind(server_fd, (struct sockaddr *)&address, sizeof(address));

  // Start listening for connections
  listen(server_fd, 3);

  while(1) {
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    uint8_t buffer[1024];
    int bytes_read = read(new_socket, buffer, 1024);
    Request *req;

    // Deserializing the incoming request
    req = request__unpack(NULL, bytes_read, buffer);
    if (req == NULL) {
      // Handle the error
    }

    // Process the request
    Response resp = RESPONSE__INIT;
    resp.status = 0; // Example status
    resp.message = "Command executed successfully"; // Example message

    // Serialize the response
    unsigned len = response__get_packed_size(&resp);
    uint8_t *buf = malloc(len);
    response__pack(&resp, buf);

    // Send the serialized response
    write(new_socket, buf, len);

    // Free the protobuf structure
    request__free_unpacked(req, NULL);
    free(buf);

    close(new_socket);
  }

  return 0;
}
