#ifndef __PARSER_H
#define __PARSER_H

#include <stdarg.h>
#include "lexer.h"

typedef enum ValueType ValueType;
typedef enum BinaryOperator BinaryOperator;

typedef enum StatementType StatementType;
typedef struct Statement Statement;
typedef struct SelectStmt SelectStmt;
typedef struct InsertStmt InsertStmt;
typedef struct CreateStmt CreateStmt;
typedef struct DeleteStmt DeleteStmt;
typedef struct UpdateStmt UpdateStmt;

typedef enum ExpressionType ExpressionType;
typedef struct Expression Expression;
typedef struct BinaryExpression BinaryExpression;
typedef struct IdentifierExpression IdentifierExpression;

enum ValueType
{
	VALUE_INT,
	VALUE_STRING
};

enum StatementType
{
	SELECT,
	CREATE,
	UPDATE,
	DELETE,
	INSERT,
	NULL_STMT
};
enum BinaryOperator
{
	GREATER_THAN,
	GREATER_THAN_OR_EQUALS,
	LESS_THAN,
	LESS_THAN_OR_EQUALS,
	NOT_EQUALS,
	EQUALS,
	BAD_OP
};

enum ExpressionType
{
	BINARY,
	IDENTIFIER
};

struct SelectStmt
{

	Expression *columns;
	uint8_t column_count;
	uint8_t column_capacity;
	char *from;
	Expression *where;
	// ...
};

struct InsertStmt
{
};

struct CreateStmt
{
	char *table_name;
};

struct DeleteStmt
{
};

struct UpdateStmt
{
};

struct Statement
{
	StatementType type;
	union
	{
		SelectStmt *select;
		DeleteStmt *delete;
		CreateStmt *create;
		InsertStmt *insert;
		UpdateStmt *update;
	};
};

struct BinaryExpression
{
	Expression *left;
	Expression *right;
	BinaryOperator operator;
};

struct IdentifierExpression
{
	ValueType type;
	union
	{
		char stringValue[255];
		int intValue;
	};
};
struct Expression
{
	ExpressionType type;
	union
	{
		BinaryExpression binary;
		IdentifierExpression identifier;
	} expr;
};

void initParser(Statement *statement, Scanner *scanner);

SelectStmt *selectStatement(Scanner *scanner, Token *token);
void freeSelect(SelectStmt *select);
void printSelect(SelectStmt *select);

CreateStmt *createStatement(Scanner *scanner, Token *token);
#endif
