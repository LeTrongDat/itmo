#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "../../proto/database.pb-c.h"
#include "../../../lab1/include/operations.h"
#include "../../../lab2/include/types.h"

void handleRequest(const Database__Request *request, Database__Response *response, Database *db);

int createTableRequest(ASTNode *root, Database__Response *response, Database *db);

int selectRequest(ASTNode *root, Database__Response *response, Database *db);

#endif // REQUEST_HANDLER_H
