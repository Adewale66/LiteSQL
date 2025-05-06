#include "../../include/parser.h"

static CreateStmt *getColumns(CreateStmt *create, Scanner *scanner, Token *token)
{
	scanToken(scanner, token);
	if (token->type != TOKEN_LPAREN)
	{
		ERROR_STMT("Expected left parenthesis", create, freeCreate);
	}
	scanToken(scanner, token);
	bool comma = false;

	while (token->type == TOKEN_IDENTIFIER)
	{
		comma = false;

		Expression column;
		column.type = COLUMN;
		column.column = ALLOCATE_MEMORY(Column, sizeof(Column));
		column.column->name = NULL;

		if (create->column_capacity < create->column_count + 1)
		{
			create->column_capacity = GROW_CAPACITY(create->column_capacity);
			create->columns = GROW_ARRAY(Expression, create->columns, create->column_capacity);
		}

		column.column->name = COPY_STRING((char *)token->literal);
		scanToken(scanner, token);
		if (token->type == TOKEN_STRING)
		{
			column.column->type = STRING;
		}
		else if (token->type == TOKEN_INT)
		{
			column.column->type = INT;
		}
		else
		{
			ERROR_STMT("Invalid column type.", create, freeCreate);
		}

		create->columns[create->column_count++] = column;

		scanToken(scanner, token); // TODO ADD A PEEK NEXT TOKEN FUNCTION
		if (token->type == TOKEN_COMMA)
		{
			comma = true;
			scanToken(scanner, token);
		}
		else
		{
			break;
		}
	}

	if (create->column_count == 0 || comma)
	{
		ERROR_STMT("unexpected identifier", create, freeCreate);
	}

	if (token->type != TOKEN_RPAREN)
	{

		ERROR_STMT("Expected right parenthesis", create, freeCreate);
	}

	scanToken(scanner, token);
	if (token->type != TOKEN_SEMICOLON)
	{
		ERROR_STMT("Invalid sql statement", create, freeCreate);
	}

	return create;
}

static CreateStmt *getTable(CreateStmt *create, Scanner *scanner, Token *token)
{
	scanToken(scanner, token);
	if (token->type != TOKEN_TABLE)
	{
		ERROR_STMT("Invalid create sql statement", create, freeCreate);
	}

	scanToken(scanner, token);
	if (token->type != TOKEN_IDENTIFIER)
	{
		ERROR_STMT("Unexpected identifier", create, freeCreate);
	}
	create->table_name = COPY_STRING((char *)token->literal);
	return getColumns(create, scanner, token);
}

CreateStmt *createStatement(Scanner *scanner, Token *token)
{
	CreateStmt *create = ALLOCATE_MEMORY(CreateStmt, sizeof(CreateStmt));
	create->column_count = 0;
	create->column_capacity = 0;
	create->columns = NULL;
	create->table_name = NULL;
	return getTable(create, scanner, token);
}

void freeCreate(CreateStmt *create)
{

	if (create->table_name != NULL)
	{
		free(create->table_name);
	}

	if (create->column_count > 0)
	{
		for (int i = 0; i < create->column_count; i++)
		{
			free(create->columns[i].column->name);
			free(create->columns[i].column);
		}
		free(create->columns);
		create->column_count = 0;
	}

	free(create);
	create = NULL;
}
