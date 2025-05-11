#include "../../include/debug.h"

static void print_create(CreateStmt *create)
{
	if (create == NULL)
	{
		return;
	}
	printf("CREATE\n");

	printf("TABLE: %s\n", create->table_name);

	for (int i = 0; i < create->column_count; i++)
	{
		printf("  COLUMN: %s\n", (char *)create->columns[i].column->name);
	}
}

static void print_select(SelectStmt *select)
{
	printf("SELECT\n");

	for (int i = 0; i < select->column_count; i++)
	{
		printf("  COLUMN: ");
		printf("%s\n", (char *)select->columns[i].literal->value);
	}

	printf("TABLE: %s\n", select->from);
}

static void print_delete(DeleteStmt *delete)
{
	printf("DELETE\n");

	printf("TABLE: %s\n", (char *)delete->table_name);
}

void print_statement(Statement *statement)
{
	switch (statement->type)
	{
	case CREATE:
		print_create(statement->create);
		break;
	case SELECT:
		print_select(statement->select);
		break;
	case DELETE:
		print_delete(statement->delete);
		break;
	case NULL_STMT:
	default:
		printf("NOT HANDLED YET\n");
	}
}

void print_token(Token *token)
{

	switch (token->type)
	{
	case TOKEN_COMMA:
		printf("COMMA");
		break;
	case TOKEN_FROM:
		printf("FROM");
		break;
	case TOKEN_SELECT:
		printf("SELECT");
		break;
	case TOKEN_IDENTIFIER:
		printf("IDENTIFER: %s", (char *)token->literal);
		break;
	case TOKEN_SEMICOLON:
		printf("SEMICOLON");
		break;
	case TOKEN_INSERT:
		printf("INSERT");
		break;
	case TOKEN_STAR:
		printf("STAR");
		break;
	case TOKEN_EOF:
		printf("EOF");
		break;
	case TOKEN_NUMBER:
		printf("NUMBER: %d", *((int *)token->literal));
		break;
	case TOKEN_LPAREN:
		printf("LEFT_PARENT");
		break;
	case TOKEN_RPAREN:
		printf("RIGHT_PAREN");
		break;
	case TOKEN_VALUES:
		printf("VALUES");
		break;
	case TOKEN_INTO:
		printf("INTO");
		break;
	default:
		printf("Token not recognized %s", (char *)token->literal);
		break;
	}
	printf("\n");
}
