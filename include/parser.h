#ifndef __PARSER_H
#define __PARSER_H

#include <stdarg.h>
#include "lexer.h"
#include "../include/expr.h"

typedef enum StatementType StatementType;
typedef struct Statement Statement;
typedef struct SelectStmt SelectStmt;
typedef struct InsertStmt InsertStmt;
typedef struct CreateStmt CreateStmt;
typedef struct DeleteStmt DeleteStmt;
typedef struct UpdateStmt UpdateStmt;

#define OPERATOR_COUNT 8

// static struct
// {
// 	TokenType type;
// 	BinaryOperator op;
// } operators[] = {
// 	{TOKEN_EQUALS, EQUALS},
// 	{TOKEN_GT, GREATER_THAN},
// 	{TOKEN_GT_EQUALS, GREATER_THAN_OR_EQUALS},
// 	{TOKEN_LT, LESS_THAN},
// 	{TOKEN_LT_EQUALS, LESS_THAN_OR_EQUALS},
// 	{TOKEN_NOT_EQUALS, NOT_EQUALS},
// 	{TOKEN_PLUS, PLUS},
// 	{TOKEN_MINUS, MINUS},
// };

enum StatementType
{
	SELECT,
	CREATE,
	UPDATE,
	DELETE,
	INSERT,
	NULL_STMT
};

struct SelectStmt
{

	Expression *columns;
	uint8_t column_count;
	uint8_t column_capacity;
	char *from;
	Expression *where;
};

struct InsertStmt
{
	char *table_name;
	Expression *columns;
	Expression *values;
	uint8_t col_count;
	uint8_t val_count;
	uint8_t col_capcity;
	uint8_t val_capcity;
};

struct CreateStmt
{
	char *table_name;
	Expression *columns;
	uint8_t column_count;
	uint8_t column_capacity;
};

struct DeleteStmt
{
	char *table_name;
	Expression *where;
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

void initParser(Statement *statement, Scanner *scanner);

SelectStmt *selectStatement(Scanner *scanner, Token *token);
void freeSelect(SelectStmt *select);
void printSelect(SelectStmt *select);

CreateStmt *createStatement(Scanner *scanner, Token *token);
void freeCreate(CreateStmt *create);
void printCreate(CreateStmt *create);

DeleteStmt *deleteStatement(Scanner *scanner, Token *token);
void freeDelete(DeleteStmt *delete);
void printDelete(DeleteStmt *delete);

#endif
