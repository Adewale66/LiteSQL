#include "parser.h"

Parser parser;
Statement statement;

static void freeSelect(SelectStmt *select)
{
	if (select != NULL)
	{

		if (select->table_name != NULL)
			free(select->table_name);
		if (select->no_of_columns != 0)
		{
			for (int i = 0; i < select->no_of_columns; i++)
			{
				free(select->column_names[i]);
			}
			free(select->column_names);
			free(select);
		}
	}
}

void freeStatment()
{
	freeTokens();
	if (statement.statemntType == TOKEN_SELECT)
	{
		freeSelect(statement.stmt.select);
		statement.stmt.select = NULL;
		statement.statemntType = TOKEN_NULL;
	}
}

static Token previous()
{
	return parser.tokenlist.tokens[parser.current - 1];
}

static Token peek()
{
	return parser.tokenlist.tokens[parser.current];
}

static bool isAtEnd()
{
	return peek().type == TOKEN_EOF;
}

static Token advance()
{
	if (!isAtEnd())
		parser.current++;
	return previous();
}

static bool check(TokenType type)
{
	if (isAtEnd())
		return false;
	return peek().type == type;
}

static bool consume(TokenType type, char *error_message)
{
	if (check(type))
	{
		advance();
		return false;
	}
	fprintf(stderr, "%s\n", error_message);
	freeStatment();
	return true;
}

static bool match(int n, ...)
{
	va_list args;
	bool checked = false;
	va_start(args, n);
	for (int i = 0; i < n; i++)
	{
		if (check(va_arg(args, TokenType)))
		{
			advance();
			checked = true;
			break;
		}
	}
	va_end(args);
	return checked;
}
static void addColumn(SelectStmt *stmt)
{
	if (previous().type == TOKEN_IDENTIFIER)
	{
		if (stmt->all_tables)
		{
			error("Invalid statement");
			freeStatment();
		}
		if (stmt->no_of_columns < stmt->capacity + 1)
		{
			int oldCapacity = stmt->capacity;
			stmt->capacity = GROW_CAPACITY(oldCapacity);
			stmt->column_names = GROW_ARRAY(char *, stmt->column_names, oldCapacity, stmt->capacity);
		}
		char *name = (char *)previous().literal;
		stmt->column_names[stmt->no_of_columns] = ALLOCATE_MEMORY(char, strlen(name) + 1, freeStatment);
		strcpy(stmt->column_names[stmt->no_of_columns], name);
		stmt->no_of_columns++;
	}
	else
	{
		if (stmt->no_of_columns != 0)
		{
			error("Invalid statement");
			freeStatment();
			return;
		}
		stmt->all_tables = true;
	}
}

static char *table()
{
	if (peek().type != TOKEN_IDENTIFIER)
	{

		error("Invalid statement near here");
		return NULL;
	}

	char *identifier_name = (char *)peek().literal;
	char *table_name = ALLOCATE_MEMORY(char, strlen(identifier_name) + 1, freeStatment);
	strcpy(table_name, identifier_name);
	advance();
	return table_name;
}

static Statement parse()
{
	Token statementType = advance();

	switch (statementType.type)
	{
	case TOKEN_SELECT:
		statement.statemntType = TOKEN_SELECT;
		SelectStmt *select = ALLOCATE_MEMORY(SelectStmt, sizeof(SelectStmt), freeStatment);
		statement.stmt.select = select;
		select->capacity = 0;
		select->no_of_columns = 0;
		select->all_tables = false;
		select->table_name = NULL;
		select->column_names = ALLOCATE_MEMORY(char *, select->capacity * sizeof(char *), freeStatment);
		bool hadError = false;
		while (match(2, TOKEN_STAR, TOKEN_IDENTIFIER))
		{
			addColumn(select);
			if (match(1, TOKEN_COMMA) && peek().type != TOKEN_IDENTIFIER)
			{
				error("Invalid statement");
				hadError = true;
				break;
			}
		}
		if (hadError)
			break;
		hadError = consume(TOKEN_FROM, "Expected FROM after statement\n");
		if (hadError)
			break;
		select->table_name = table();
		if (select->table_name == NULL)
		{
			freeStatment();
			break;
		}
		// this is where other keywords like limit, where, subqueries... would be checked (future TODO)
		hadError = consume(TOKEN_SEMICOLON, "Statment must end with a semi colon\n");
		if (hadError)
			break;
		break;
	case TOKEN_INSERT:
		statement.statemntType = TOKEN_INSERT;
		InsertStmt *insert = ALLOCATE_MEMORY(InsertStmt, sizeof(InsertStmt), freeStatment);
		statement.stmt.insert = insert;
		break;
	default:
		printf("unknown staement type\n");
		break;
	}
	freeTokens();
	return statement;
}

Statement intiParser(TokenList tokens)
{
	parser.tokenlist = tokens;
	parser.current = 0;
	return parse();
}
