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

static char *table()
{
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
static char *addColumn(SelectStmt *stmt)
{
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
		select->column_names = 0;
		while (match(1, TOKEN_IDENTIFIER))
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
	return statement;
}

void intiParser(TokenList tokens)
{
	parser.tokenlist = tokens;
	parser.current = 0;
	parse();
}
