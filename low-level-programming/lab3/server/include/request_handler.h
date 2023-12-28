#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "../../proto/database.pb-c.h"
#include "../../../lab1/include/operations.h"

void handle_request(const Database__Request *request, Database__Response *response, Database *db);

#endif // REQUEST_HANDLER_H
