#ifndef __PARSER_H
#define __PARSER_H

#include <stdarg.h>
#include "lexer.h"
#include "errno.h"

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
	int current;

} Parser;

ASTNode *intiParser(TokenList tokens);
void freeNode(ASTNode *node);
void printAST(ASTNode *node, int indent);

void cleanup();

// Helper functions

Token previous();
Token peek();
Token consume(TokenType type, char *errorMessage);
bool isAtEnd();
Token advance();
bool match(int n, ...);
bool check(TokenType type);

ASTNode *selectStatement();
#endif
