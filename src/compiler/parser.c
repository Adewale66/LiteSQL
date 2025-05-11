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

void free_statement(Statement *statement)
{
	switch (statement->type)
	{
	case CREATE:
		free_create(statement->create);
		break;
	case SELECT:
		free_select(statement->select);
		break;
	case DELETE:
		free_delete(statement->delete);
		break;
	case NULL_STMT:
	default:
		return;
	}
}

Statement *prepare_statement(Scanner *scanner)
{
	Token token;
	token.literal = NULL;
	token.type = TOKEN_NULL;

	Statement *stmt = ALLOCATE_MEMORY(Statement, sizeof(Statement));

	scanToken(scanner, &token);
	switch (token.type)
	{
	case TOKEN_SELECT:
		stmt->type = SELECT;
		stmt->select = NULL;
		stmt->select = select_statement(scanner, &token);
		if (stmt->select == NULL)
		{
			stmt->type = NULL_STMT;
		}
		break;
	case TOKEN_CREATE:
		stmt->type = CREATE;
		stmt->create = NULL;
		stmt->create = create_statement(scanner, &token);
		if (stmt->create == NULL)
		{
			stmt->type = NULL_STMT;
		}
		break;
	case TOKEN_DELETE:
		stmt->type = DELETE;
		stmt->delete = NULL;
		stmt->delete = delete_statement(scanner, &token);
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
	return stmt;
}
