#ifndef L2_TYPES_H
#define L2_TYPES_H

typedef enum {
    NODE_TYPE_SELECT,
    NODE_TYPE_COLUMN_LIST,
    NODE_TYPE_COLUMN,
    NODE_TYPE_COLUMN_NAME,
    NODE_TYPE_FROM,
    NODE_TYPE_JOIN,
    NODE_TYPE_WHERE,
    NODE_TYPE_CONDITION,
    NODE_TYPE_LITERAL,
    NODE_TYPE_EXPRESSION,
    NODE_TYPE_DATA_TYPE,
    NODE_TYPE_COLUMN_DEF,
    NODE_TYPE_TABLE,
    NODE_TYPE_COLUMN_CONSTRAINT,
    NODE_TYPE_DELETE,
    NODE_TYPE_VALUE_LIST,
    NODE_TYPE_INSERT,
    NODE_TYPE_UPDATE,
    NODE_TYPE_CREATE_TABLE,
    NODE_TYPE_COLUMN_DEF_LIST,
    NODE_TYPE_NE,
    NODE_TYPE_GT,
    NODE_TYPE_LT,
    NODE_TYPE_GE,
    NODE_TYPE_LE,
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char *tokenName;
    char *value;     
    struct ASTNode **children; 
    int child_count; 
} ASTNode;

ASTNode* createNode(ASTNodeType type, char *tokenName, char *value, int child_count, ...);

void mergeNodes(ASTNode *target, ASTNode *source);

void appendChild(ASTNode *parent, ASTNode *child);

void freeAST(ASTNode *node);

void printAST(ASTNode *node, int level);

#endif