#include "../../include/parser.h"

DeleteStmt *getTable(DeleteStmt *delete, Scanner *scanner, Token *token)
{
	scanToken(scanner, token);
	if (token->type != TOKEN_FROM)
	{
		ERROR_STMT("Expected \"FROM\" keyword", delete, freeDelete)
	}
	scanToken(scanner, token);
	if (token->type != TOKEN_IDENTIFIER)
	{
		ERROR_STMT("Unexpected identifier", delete, freeDelete)
	}
	delete->table_name = COPY_STRING((char *)token->literal);
	scanToken(scanner, token);

	if (token->type != TOKEN_SEMICOLON)
	{
		ERROR_STMT("Invalid sql statement", delete, freeDelete)
	}

	return delete;
}

DeleteStmt *deleteStatement(Scanner *scanner, Token *token)
{
	DeleteStmt *delete = ALLOCATE_MEMORY(DeleteStmt, sizeof(DeleteStmt));
	delete->table_name = NULL;
	delete->where = NULL;

	return getTable(delete, scanner, token);
}

void freeDelete(DeleteStmt *delete)
{
	if (delete == NULL)
		return;

	if (delete->table_name != NULL)
	{
		free(delete->table_name);
	}
	free(delete);
	delete = NULL;
}
