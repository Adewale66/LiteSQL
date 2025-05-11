#include "../../include/parser.h"

DeleteStmt *getTable(DeleteStmt *delete, Scanner *scanner, Token *token)
{
	scanToken(scanner, token);
	if (token->type != TOKEN_FROM)
	{
		ERROR_STMT("Expected \"FROM\" keyword", delete, free_delete)
	}
	scanToken(scanner, token);
	if (token->type != TOKEN_IDENTIFIER)
	{
		ERROR_STMT("Unexpected identifier", delete, free_delete)
	}
	delete->table_name = COPY_STRING((char *)token->literal);
	scanToken(scanner, token);

	if (token->type != TOKEN_SEMICOLON)
	{
		ERROR_STMT("Invalid sql statement", delete, free_delete)
	}

	return delete;
}

DeleteStmt *delete_statement(Scanner *scanner, Token *token)
{
	DeleteStmt *delete = ALLOCATE_MEMORY(DeleteStmt, sizeof(DeleteStmt));
	delete->table_name = NULL;
	delete->where = NULL;

	return getTable(delete, scanner, token);
}

void free_delete(DeleteStmt *delete)
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
