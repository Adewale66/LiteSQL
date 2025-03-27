#ifndef __PARSER_H
#define __PARSER_H

#include <stdarg.h>
#include "lexer.h"

typedef enum StatementType StatementType;
typedef struct Statement Statement;
typedef struct SelectStmt SelectStmt;
typedef struct InsertStmt InsertStmt;
typedef struct CreateStmt CreateStmt;
typedef struct DeleteStmt DeleteStmt;
typedef struct UpdateStmt UpdateStmt;

typedef struct BinaryExpression BinaryExpression;
typedef struct IdentifierExpression IdentifierExpression;
typedef struct ColumnExpression ColumnExpression;

// Column types
#define INT 0x01
#define STRING 0x02

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

	IdentifierExpression *columns;
	uint8_t column_count;
	uint8_t column_capacity;
	char *from;
	IdentifierExpression *where;
	// ...
};

struct InsertStmt
{
};

struct CreateStmt
{
	char *table_name;
	ColumnExpression *columns;
	uint8_t column_count;
	uint8_t column_capacity;
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

struct IdentifierExpression
{
	uint8_t type;
	union
	{
		char stringValue[255];
		int intValue;
	};
};

struct ColumnExpression
{
	char column_name[255];
	uint8_t type;
};

void initParser(Statement *statement, Scanner *scanner);

SelectStmt *selectStatement(Scanner *scanner, Token *token);
void freeSelect(SelectStmt *select);
void printSelect(SelectStmt *select);

CreateStmt *createStatement(Scanner *scanner, Token *token);
void freeCreate(CreateStmt *create);
void printCreate(CreateStmt *create);
#endif
