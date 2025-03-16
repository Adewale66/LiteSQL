#ifndef __PARSER_H
#define __PARSER_H

#include <stdarg.h>
#include "lexer.h"
#include "error.h"

typedef enum NodeType NodeType;
typedef enum ValueType ValueType;
typedef struct ASTNode ASTNode;
typedef struct Value Value;

enum NodeType
{
	SELECT,
	TABLE,
	COLUMN,
	WHERE,
	COMPARISON,
	ID,
	NUMBER
};

enum ValueType
{
	VALUE_INT,
	VALUE_STRING
};

struct Value
{
	ValueType type;
	union
	{
		char *stringValue;
		int intValue;
	};
};

struct ASTNode
{
	NodeType type;
	Value value;
	char *comparisonOperator;
	ASTNode *left;
	ASTNode *right;
	ASTNode **children;
	int childCount;
};

typedef struct
{
	TokenList tokenlist;
	ASTNode *root;
	int current;

} Parser;

ASTNode *initParser(TokenList tokens);
void freeNode(ASTNode *node);
void printAST(ASTNode *node, int indent);

// Helper functions

Token previous();
Token peek();
void consume(TokenType type, char *errorMessage, bool *error);
bool isAtEnd();
Token advance();
bool match(int n, ...);
bool check(TokenType type);

void memoryCleanup();

ASTNode *selectStatement();
#endif
