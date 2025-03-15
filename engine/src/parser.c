#include "parser.h"

Parser parser;
Statement statement;

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

static Token consume(TokenType type, char *error_message)
{
	if (check(type))
		return advance();
	fprintf(stderr, "%s\n", error_message); // TODO FREE EVERYTHING
	exit(6);
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
			fprintf(stderr, "Invalid select\n"); // Come up with something better
			freeTokens();
			exit(4);
		}
		if (stmt->no_of_columns < stmt->capacity + 1)
		{
			int oldCapacity = stmt->capacity;
			stmt->capacity = GROW_CAPACITY(oldCapacity);
			stmt->column_names = realloc(stmt->column_names, stmt->capacity * sizeof(char *));
			if (stmt->column_names == NULL)
			{
				fprintf(stderr, "Could not malloc\n");
				freeTokens();
				exit(9);
			}
		}
		char *name = (char *)previous().literal;
		stmt->column_names[stmt->no_of_columns] = malloc(strlen(name) + 1);
		if (stmt->column_names[stmt->no_of_columns] == NULL)
		{
			fprintf(stderr, "Could not malloc\n");
			freeTokens();
			exit(10);
		}
		strcpy(stmt->column_names[stmt->no_of_columns], name);
		stmt->no_of_columns++;
	}
	else
	{
		if (stmt->no_of_columns != 0)
		{
			fprintf(stderr, "Invalid statement\n"); // Something better
			freeTokens();
			exit(4);
		}
		stmt->all_tables = true;
	}
}

static char *table()
{
	if (peek().type != TOKEN_IDENTIFIER)
	{
		exit(69); // HANDLE THIS
	}

	char *identifier_name = (char *)peek().literal;
	char *table_name = malloc(strlen(identifier_name) + 1);
	if (table_name == NULL)
	{
		exit(69); // HANDLE THIS
	}
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
		SelectStmt *select = (SelectStmt *)malloc(sizeof(SelectStmt));
		if (select == NULL)
		{
			fprintf(stderr, "Could not malloc\n");
			freeTokens();
			exit(4);
		}
		select->capacity = 0;
		select->no_of_columns = 0;
		select->all_tables = false;
		select->column_names = malloc(select->capacity * sizeof(char *));
		if (select->column_names == NULL)
		{
			fprintf(stderr, "Could not malloc\n");
			freeTokens();
			exit(7);
		}
		while (match(2, TOKEN_STAR, TOKEN_IDENTIFIER))
		{
			addColumn(select);
			if (!match(1, TOKEN_COMMA))
				break;
		}
		consume(TOKEN_FROM, "Expected FROM after statement\n");
		select->table_name = table();
		// this is where other keywords like limit, where, subqueries... would be checked (TODO)
		consume(TOKEN_SEMICOLON, "Statment must end with a semi colon\n");
		statement.statemntType = TOKEN_SELECT;
		statement.stmt.select = select;
		break;
	case TOKEN_INSERT:
		InsertStmt *insert = (InsertStmt *)malloc(sizeof(InsertStmt));
		if (insert == NULL)
		{
			fprintf(stderr, "Could not malloc\n");
			freeTokens();
			exit(4);
		}
		statement.statemntType = TOKEN_INSERT;
		statement.stmt.insert = insert;
		break;
	default:
		printf("unknown staement type\n");
		break;
	}
	// freeTokens();
	return statement;
}

Statement intiParser(TokenList tokens)
{
	parser.tokenlist = tokens;
	parser.current = 0;
	return parse();
}
