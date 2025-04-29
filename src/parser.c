#include "../include/parser.h"

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
