#include <strings.h>
#include "parser.h"

static struct
{
	TokenType type;
	BinaryOperator op;
} operators[] = {
	{TOKEN_EQUALS, EQUALS},
	{TOKEN_GT, GREATER_THAN},
	{TOKEN_GT_EQUALS, GREATER_THAN_OR_EQUALS},
	{TOKEN_LT, LESS_THAN},
	{TOKEN_LT_EQUALS, LESS_THAN_OR_EQUALS},
	{TOKEN_NOT_EQUALS, NOT_EQUALS}};

static int opCount = 6;

static Expression *createExpr(ExpressionType type)
{
	Expression *expr = ALLOCATE_MEMORY(Expression, sizeof(Expression));
	expr->type = type;
	return expr;
}

static BinaryOperator getOp(TokenType type)
{
	for (int i = 0; i < opCount; i++)
	{
		if (operators[i].type == type)
			return operators[i].op;
	}
	return BAD_OP;
}
static void parseWhere(SelectStmt *select, Scanner *scanner, Token *token) //(TODO FIX)
{

	scanToken(scanner, token);
	Expression *expression = createExpr(BINARY);

	if (token->type == TOKEN_NUMBER)
	{
		Expression *left = createExpr(IDENTIFIER);
		left->expr.identifier.intValue = *((int *)token->literal);
		expression->expr.binary.left = left;
	}
	else if (token->type == TOKEN_IDENTIFIER)
	{
		Expression *left = createExpr(IDENTIFIER);
		strncpy(left->expr.identifier.stringValue, (char *)token->literal, strlen((char *)token->literal));
		expression->expr.binary.left = left;
	}
	else
	{
		fprintf(stderr, "Error: Unexpected identifier\n");
		// handle this
		return;
	}

	scanToken(scanner, token);
	BinaryOperator op = getOp(token->type);

	if (op == BAD_OP)
	{
		// handle this
		return;
	}
	expression->expr.binary.operator= op;
	scanToken(scanner, token);

	if (token->type == TOKEN_NUMBER)
	{
		Expression *right = createExpr(IDENTIFIER);
		right->expr.identifier.intValue = *((int *)token->literal);
		expression->expr.binary.right = right;
	}
	else if (token->type == TOKEN_IDENTIFIER)
	{
		Expression *right = createExpr(IDENTIFIER);
		strncpy(right->expr.identifier.stringValue, (char *)token->literal, strlen((char *)token->literal));
		expression->expr.binary.right = right;
	}
	else
	{
		fprintf(stderr, "Error: Unexpected identifier\n");
		// handle this
		return;
	}
	select->where = expression;
}

static SelectStmt *checkClause(SelectStmt *select, Scanner *scanner, Token *token)
{
	scanToken(scanner, token);

	if (token->type == TOKEN_WHERE)
	{
		parseWhere(select, scanner, token);
	}
	if (token->type != TOKEN_SEMICOLON)
	{
		// handle this
		fprintf(stderr, "Error: expected semicolon.\n");
		free(select);
		return NULL;
	}
	return select;
}

static SelectStmt *parseTable(SelectStmt *select, Scanner *scanner, Token *token)
{

	if (token->type != TOKEN_FROM)
	{

		freeSelect(select);
		fprintf(stderr, "Error: Invalid SQL statement\n");
		return NULL;
	}
	scanToken(scanner, token);
	if (token->type != TOKEN_IDENTIFIER)
	{
		// handle this
		fprintf(stderr, "Error: Unexpected identifier\n");
		free(select);
		return NULL;
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
		Expression column;
		if (select->column_capacity < select->column_count + 1)
		{
			uint8_t oldCapacity = select->column_capacity;
			select->column_capacity = GROW_CAPACITY(oldCapacity);
			select->columns = GROW_ARRAY(Expression, select->columns, select->column_capacity);
		}
		column.type = IDENTIFIER;
		column.expr.identifier.type = VALUE_STRING;
		size_t max_len = sizeof(column.expr.identifier.stringValue) - 1;
		strncpy(column.expr.identifier.stringValue, token->literal, max_len);
		column.expr.identifier.stringValue[max_len] = '\0';

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
		// handle this
		fprintf(stderr, "Error: Unexpected identifier\n");
		free(select);
		return NULL;
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
}

void printSelect(SelectStmt *select)
{
	printf("SELECT\n");

	for (int i = 0; i < select->column_count; i++)
	{
		printf("  ");
		printf("COLUMN: ");
		if (select->columns[i].type == IDENTIFIER)
		{
			if (select->columns[i].expr.identifier.type == VALUE_STRING)
				printf("%s\n", select->columns[i].expr.identifier.stringValue);
			else

				printf("%d\n", select->columns[i].expr.identifier.intValue);
		}
	}

	printf("TABLE: %s\n", select->from);
}
