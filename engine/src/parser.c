#include "parser.h"

Parser parser;
Statement statement;

void freeStatment()
{
	freeTokens();
	if (statement.statemntType == TOKEN_SELECT)
	{
		if (statement.stmt.select->table_name != NULL)
			free(statement.stmt.select->table_name);
		if (statement.stmt.select->no_of_columns != 0)
		{
			for (int i = 0; i < statement.stmt.select->no_of_columns; i++)
			{
				free(statement.stmt.select->column_names[i]);
			}
			free(statement.stmt.select->column_names);
			free(statement.stmt.select);
		}
		statement.stmt.select->no_of_columns = 0;
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
		stmt->column_names[stmt->no_of_columns] = ALLOCATE_MEMORY(char, strlen(name) + 1);
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
		freeStatment();
		error("Invalid statemet");
	}

	char *identifier_name = (char *)peek().literal;
	char *table_name = ALLOCATE_MEMORY(char, strlen(identifier_name) + 1);
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
		SelectStmt *select = ALLOCATE_MEMORY(SelectStmt, sizeof(SelectStmt)); // function pointet maybe?
		select->capacity = 0;
		select->no_of_columns = 0;
		select->all_tables = false;
		select->column_names = ALLOCATE_MEMORY(char *, select->capacity * sizeof(char *));
		while (match(2, TOKEN_STAR, TOKEN_IDENTIFIER))
		{
			addColumn(select);
			if (match(1, TOKEN_COMMA) && peek().type != TOKEN_IDENTIFIER)
			{
				error("Invalid statement");
				freeStatment();
			}
		}
		consume(TOKEN_FROM, "Expected FROM after statement\n");
		select->table_name = table();
		// this is where other keywords like limit, where, subqueries... would be checked (TODO)
		consume(TOKEN_SEMICOLON, "Statment must end with a semi colon\n");
		statement.statemntType = TOKEN_SELECT;
		statement.stmt.select = select;
		break;
	case TOKEN_INSERT:
		InsertStmt *insert = ALLOCATE_MEMORY(InsertStmt, sizeof(InsertStmt));
		statement.statemntType = TOKEN_INSERT;
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
