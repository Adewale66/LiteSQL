#include "parser.h"

// CREATE TABLE users ( id int );

static void *errorCreate(const char *message, CreateStmt *create)
{
	fprintf(stderr, "Error: %s\n", message);
	freeCreate(create);
	return NULL;
}

static CreateStmt *getColumns(CreateStmt *create, Scanner *scanner, Token *token)
{
	scanToken(scanner, token);
	if (token->type != TOKEN_LPAREN)
	{
		return errorCreate("Expected left parenthesis", create);
	}
	scanToken(scanner, token);
	bool comma = false;

	while (token->type == TOKEN_IDENTIFIER)
	{
		comma = false;

		ColumnExpression column;

		if (create->column_capacity < create->column_count + 1)
		{
			create->column_capacity = GROW_CAPACITY(create->column_capacity);
			create->columns = GROW_ARRAY(ColumnExpression, create->columns, create->column_capacity);
		}
		size_t max_len = sizeof(column.column_name) - 1;
		strncpy(column.column_name, (char *)token->literal, max_len);
		column.column_name[max_len] = '\0';

		scanToken(scanner, token);
		if (token->type == TOKEN_STRING)
		{
			column.type = STRING;
		}
		else if (token->type == TOKEN_INT)
		{
			column.type = INT;
		}
		else
		{
			return errorCreate("Invalid column type.", create);
		}

		create->columns[create->column_count++] = column;

		scanToken(scanner, token);
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
		return errorCreate("unexpected identifier", create);
	}

	if (token->type != TOKEN_RPAREN)
	{

		return errorCreate("Expected right parenthesis", create);
	}

	scanToken(scanner, token);
	if (token->type != TOKEN_SEMICOLON)
	{
		return errorCreate("Invalid sql statement", create);
	}

	return create;
}

static CreateStmt *getTable(CreateStmt *create, Scanner *scanner, Token *token)
{
	scanToken(scanner, token);
	if (token->type != TOKEN_TABLE)
	{
		return errorCreate("Invalid create sql statement", create);
	}

	scanToken(scanner, token);
	if (token->type != TOKEN_IDENTIFIER)
	{
		return errorCreate("Unexpected identifier", create);
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
		free(create->columns);
		create->column_count = 0;
	}

	free(create);
	create = NULL;
}

void printCreate(CreateStmt *create)
{
	if (create == NULL)
	{
		return;
	}
	printf("CREATE\n");

	printf("TABLE: %s\n", create->table_name);

	for (int i = 0; i < create->column_count; i++)
	{
		printf("  COLUMN: %s\n", create->columns[i].column_name);
	}
}
