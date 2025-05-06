#ifndef __PARSER_H
#define __PARSER_H

#include <stdarg.h>

#include "lexer.h"
#include "expr.h"

typedef enum StatementType StatementType;
typedef struct Statement Statement;
typedef struct SelectStmt SelectStmt;
typedef struct InsertStmt InsertStmt;
typedef struct CreateStmt CreateStmt;
typedef struct DeleteStmt DeleteStmt;
typedef struct UpdateStmt UpdateStmt;

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
void freeStatement(Statement statement);
BinaryOperator getOp(TokenType type);

SelectStmt *selectStatement(Scanner *scanner, Token *token);
void freeSelect(SelectStmt *select);

CreateStmt *createStatement(Scanner *scanner, Token *token);
void freeCreate(CreateStmt *create);

DeleteStmt *deleteStatement(Scanner *scanner, Token *token);
void freeDelete(DeleteStmt *delete);

#endif
