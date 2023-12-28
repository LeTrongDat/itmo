#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "database.pb-c.h"

void handle_request(const Database__Request *request, Database__Response *response);

#endif // REQUEST_HANDLER_H
