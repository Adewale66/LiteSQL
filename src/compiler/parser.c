#include "../../include/parser.h"
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
	{TOKEN_NOT_EQUALS, NOT_EQUALS},
	{TOKEN_PLUS, PLUS},
	{TOKEN_MINUS, MINUS},
	{TOKEN_DIVIDE, DIVIDE},
	{TOKEN_STAR, MULTIPLY},
};

BinaryOperator getOp(TokenType type)
{
	size_t op_count = sizeof(operators) / sizeof(operators[0]);
	for (size_t i = 0; i < op_count; i++)
	{
		if (operators[i].type == type)
			return operators[i].op;
	}
	return BAD_OP;
}

void freeStatement(Statement statement)
{
	switch (statement.type)
	{
	case CREATE:
		freeCreate(statement.create);
		break;
	case SELECT:
		freeSelect(statement.select);
		break;
	case DELETE:
		freeDelete(statement.delete);
		break;
	case NULL_STMT:
	default:
		return;
	}
}

void initParser(Statement *stmt, Scanner *scanner)
{
	Token token;
	token.literal = NULL;
	token.type = TOKEN_NULL;

	scanToken(scanner, &token);
	switch (token.type)
	{
	case TOKEN_SELECT:
		stmt->type = SELECT;
		stmt->select = NULL;
		stmt->select = selectStatement(scanner, &token);
		if (stmt->select == NULL)
		{
			stmt->type = NULL_STMT;
		}
		break;
	case TOKEN_CREATE:
		stmt->type = CREATE;
		stmt->create = NULL;
		stmt->create = createStatement(scanner, &token);
		if (stmt->create == NULL)
		{
			stmt->type = NULL_STMT;
		}
		break;
	case TOKEN_DELETE:
		stmt->type = DELETE;
		stmt->delete = NULL;
		stmt->delete = deleteStatement(scanner, &token);
		if (stmt->delete == NULL)
		{
			stmt->type = NULL_STMT;
		}
		break;
	default:
		stmt->type = NULL_STMT;
		break;
	}
	if (token.type != TOKEN_NUMBER && token.type != TOKEN_NULL)
	{
		free(token.literal);
	}
}
