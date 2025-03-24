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
		stmt->select.column_capacity = 0;
		stmt->select.column_count = 0;
		stmt->select.columns = NULL;
		stmt->select.from = NULL;
		selectStatement(&(stmt->select), scanner, &token);
		break;

	default:
		break;
	}
	if (token.type != TOKEN_NUMBER && token.type != TOKEN_NULL)
	{
		free(token.literal);
	}
}
