#include "parser.h"

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
		break;
	case TOKEN_CREATE:
		stmt->type = CREATE;
		stmt->create = NULL;
		stmt->create = createStatement(scanner, &token);
	default:
		break;
	}
	if (token.type != TOKEN_NUMBER && token.type != TOKEN_NULL)
	{
		free(token.literal);
	}
}
