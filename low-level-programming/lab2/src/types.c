#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

ASTNode* createNode(ASTNodeType type, char *tokenName, char *value, int child_count, ...) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->tokenName = strdup(tokenName);
    node->value = value ? strdup(value) : NULL;
    node->child_count = child_count;
    node->children = NULL;

    if (child_count > 0) {
        va_list args;
        va_start(args, child_count);
        node->children = (ASTNode**)malloc(sizeof(ASTNode*) * child_count);
        for (int i = 0; i < child_count; i++) {
            node->children[i] = va_arg(args, ASTNode*);
        }
        va_end(args);
    }

    return node;
}

void mergeNodes(ASTNode *target, ASTNode *source) {
    if (!target || !source || target->type != source->type) return;


    int newChildCount = target->child_count + source->child_count;
    target->children = (ASTNode**)realloc(target->children, sizeof(ASTNode*) * newChildCount);

    for (int i = 0; i < source->child_count; i++) {
        target->children[target->child_count + i] = source->children[i];
    }
    
    target->child_count = newChildCount;
}

void freeAST(ASTNode *node) {
    if (!node) return;
    
    for (int i = 0; i < node->child_count; i++) {
        freeAST(node->children[i]);
    }

    if (node->children) free(node->children);
    if (node->tokenName) free(node->tokenName);
    if (node->value) free(node->value);

    free(node);
}

char* appendStr(char *orig, const char *fmt, ...) {
    va_list args;
    char *buffer;
    int needed, new_len;

    va_start(args, fmt);
    needed = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    new_len = (orig ? strlen(orig) : 0) + needed + 1;
    buffer = malloc(new_len);

    if (orig) {
        strcpy(buffer, orig);
        free(orig);
    }

    va_start(args, fmt);
    vsnprintf(buffer + (new_len - needed - 1), needed + 1, fmt, args);
    va_end(args);

    return buffer;
}

char* toString(ASTNode *node, int level) {
    char *result = NULL;

    if (!node) return NULL;

    for (int i = 0; i < level; i++) {
        if (i == level - 1) result = appendStr(result, "|-- ");
        else result = appendStr(result, "|   ");
    }

    result = appendStr(result, "%s", node->tokenName);
    if (node->value) result = appendStr(result, ": %s", node->value);
    result = appendStr(result, "\n");

    for (int i = 0; i < node->child_count; i++) {
        char *childStr = toString(node->children[i], level + 1);
        if (childStr) {
            result = appendStr(result, "%s", childStr);
            free(childStr);
        }
    }

    return result;
}
