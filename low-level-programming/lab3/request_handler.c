#include "request_handler.h"
#include <string.h>

void handle_request(const Database__Request *request, Database__Response *response) {
    response->message = strdup("Request processed successfully");
    response->success = 1;
}
