#ifndef __STMT_H
#define __STMT_H

#include "lexer.h"

typedef struct
{
	char **column_names;
	int no_of_columns;
	int capacity;
	char *table_name;
	bool all_tables;
} SelectStmt;

typedef struct
{

} InsertStmt;

typedef struct
{
	TokenType statemntType;
	union
	{
		SelectStmt *select;
		InsertStmt *insert;
	} stmt;

} Statement;

#endif
