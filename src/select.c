#include <strings.h>
#include "parser.h"

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
// 	{TOKEN_NOT_EQUALS, NOT_EQUALS}};

// static int opCount = 6;

// static BinaryOperator getOp(TokenType type)
// {
// 	for (int i = 0; i < opCount; i++)
// 	{
// 		if (operators[i].type == type)
// 			return operators[i].op;
// 	}
// 	return BAD_OP;
// }

static void *errorSelect(const char *message, SelectStmt *select)
{
	fprintf(stderr, "Error: %s.\n", message);
	freeSelect(select);
	return NULL;
}

static SelectStmt *checkClause(SelectStmt *select, Scanner *scanner, Token *token)
{
	scanToken(scanner, token);
	if (token->type != TOKEN_SEMICOLON)
	{
		return errorSelect("expected semicolon.", select);
	}
	return select;
}

static SelectStmt *parseTable(SelectStmt *select, Scanner *scanner, Token *token)
{

	if (token->type != TOKEN_FROM)
	{
		return errorSelect("Invalid sql statement.", select);
	}
	scanToken(scanner, token);
	if (token->type != TOKEN_IDENTIFIER)
	{
		return errorSelect("Invalid sql statement.", select);
	}
	select->from = COPY_STRING(token->literal);

	return checkClause(select, scanner, token);
}

static SelectStmt *parseColumns(SelectStmt *select, Scanner *scanner, Token *token)
{
	scanToken(scanner, token);
	bool comma = false;

	while (token->type == TOKEN_IDENTIFIER)
	{
		comma = false;
		IdentifierExpression column;
		if (select->column_capacity < select->column_count + 1)
		{
			select->column_capacity = GROW_CAPACITY(select->column_capacity);
			select->columns = GROW_ARRAY(IdentifierExpression, select->columns, select->column_capacity);
		}
		column.type = STRING;
		size_t max_len = sizeof(column.stringValue) - 1;
		strncpy(column.stringValue, (char *)token->literal, max_len);
		column.stringValue[max_len] = '\0';

		select->columns[select->column_count++] = column;

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

	if (select->column_count == 0 || comma)
	{
		return errorSelect("Unpected identifier", select);
	}

	return parseTable(select, scanner, token);
}

SelectStmt *selectStatement(Scanner *scanner, Token *token)
{
	SelectStmt *select = ALLOCATE_MEMORY(SelectStmt, sizeof(SelectStmt));
	select->column_capacity = 0;
	select->column_count = 0;
	select->columns = NULL;
	select->from = NULL;
	return parseColumns(select, scanner, token);
}

void freeSelect(SelectStmt *select)
{

	if (select == NULL)
		return;

	if (select->column_count > 0)
	{
		free(select->columns);
		select->column_count = 0;
	}

	if (select->from != NULL)
		free(select->from);
	free(select);
	select = NULL;
}

void printSelect(SelectStmt *select)
{
	printf("SELECT\n");

	for (int i = 0; i < select->column_count; i++)
	{
		printf("  COLUMN: ");

		if (select->columns[i].type == STRING)
			printf("%s\n", select->columns[i].stringValue);
		else
			printf("%d\n", select->columns[i].intValue);
	}

	printf("TABLE: %s\n", select->from);
}
