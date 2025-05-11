#include "../../include/parser.h"

typedef struct
{
	Expression *expression;
	int count;
	int cap;
} Columns;

static Columns *getColumns(Scanner *scanner, Token *token)
{
	bool valid = consume(scanner, token, TOKEN_LPAREN, "Expected left parenthesis");
	if (!valid)
	{
		return NULL;
	}

	Columns *cols = ALLOCATE_MEMORY(Columns, sizeof(Columns));
	cols->cap = 0;
	cols->count = 0;

	scanToken(scanner, token);
	bool comma = false;

	while (token->type == TOKEN_IDENTIFIER)
	{
		comma = false;

		Expression column;
		column.type = COLUMN;
		column.column = ALLOCATE_MEMORY(Column, sizeof(Column));
		column.column->name = NULL;

		if (cols->cap < cols->count + 1)
		{
			cols->cap = GROW_CAPACITY(cols->cap);
			cols->expression = GROW_ARRAY(Expression, cols->expression, cols->cap);
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
		else if (token->type == TOKEN_BOOL)
		{
			column.column->type = BOOL;
		}
		else
		{
			fprintf(stderr, "Invalid column type.");
			// FREE SOMETHING HERE
			return NULL;
		}

		cols->expression[cols->count++] = column;

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

	if (cols->count == 0 || comma)
	{
		// FREE SOMETHING HERE
		fprintf(stderr, "unexpected identifier");
		return NULL;
	}

	if (token->type != TOKEN_RPAREN)
	{
		// FREE SOMETHING HERE
		fprintf(stderr, "Expected right parenthesis");
		return NULL;
	}

	valid = consume(scanner, token, TOKEN_SEMICOLON, "Invalid sql statement");
	if (!valid)
	{
		// FREE SOMETHING HERE
		return NULL;
	}

	return cols;
}

static char *getTable(Scanner *scanner, Token *token)
{
	bool valid = consume(scanner, token, TOKEN_TABLE, "Invalid create sql statement\n");
	if (!valid)
	{
		return NULL;
	}

	valid = consume(scanner, token, TOKEN_IDENTIFIER, "Unexpected identifier\n");
	if (!valid)
	{
		return NULL;
	}
	char *table_name = COPY_STRING((char *)token->literal);
	return table_name;
}

CreateStmt *create_statement(Scanner *scanner, Token *token)
{

	char *table_name = getTable(scanner, token);
	if (table_name == NULL)
	{
		return NULL;
	}
	Columns *cols = getColumns(scanner, token);
	if (cols == NULL)
	{
		return NULL;
	}
	CreateStmt *create = ALLOCATE_MEMORY(CreateStmt, sizeof(CreateStmt));
	create->column_count = cols->count;
	create->columns = cols->expression;
	create->table_name = table_name;

	free(cols);

	return create;
}

void free_create(CreateStmt *create)
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
