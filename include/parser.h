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

Statement *prepare_statement(Scanner *scanner);
void free_statement(Statement *statement);
BinaryOperator get_operator(TokenType type);

SelectStmt *select_statement(Scanner *scanner, Token *token);
void free_select(SelectStmt *select);

CreateStmt *create_statement(Scanner *scanner, Token *token);
void free_create(CreateStmt *create);

DeleteStmt *delete_statement(Scanner *scanner, Token *token);
void free_delete(DeleteStmt *delete);

#endif
