#include "../../include/lexer.h"

static struct
{
	char *keyword;
	TokenType type;
} keywords[] = {
	{"select", TOKEN_SELECT},
	{"insert", TOKEN_INSERT},
	{"create", TOKEN_CREATE},
	{"delete", TOKEN_DELETE},
	{"update", TOKEN_UPDATE},
	{"from", TOKEN_FROM},
	{"where", TOKEN_WHERE},
	{"values", TOKEN_VALUES},
	{"set", TOKEN_SET},
	{"into", TOKEN_INTO},
	{"table", TOKEN_TABLE},
	{"string", TOKEN_STRING},
	{"int", TOKEN_INT},
};

static bool isAtEnd(Scanner *scanner)
{
	return scanner->current >= scanner->source_length;
}

static char *substring(char *source, int start, int length)
{
	char *sub = ALLOCATE_MEMORY(char, length + 1);
	memcpy(sub, source + start, length);
	sub[length] = '\0';
	return sub;
}

static bool match(char expected, Scanner *scanner)
{
	if (isAtEnd(scanner))
		return false;
	if (scanner->source[scanner->current] != expected)
		return false;
	scanner->current++;
	return true;
}

static TokenType findKeyword(char *text)
{
	size_t keywordCount = sizeof(keywords) / sizeof(keywords[0]);
	for (size_t i = 0; i < keywordCount; i++)
	{
		if (strcasecmp(keywords[i].keyword, text) == 0)
			return keywords[i].type;
	}

	return TOKEN_NULL;
}

void initScanner(Scanner *scanner, char *source, size_t length)
{
	scanner->source = source;
	scanner->start = 0;
	scanner->source_length = length;
	scanner->current = 0;
}

static void addToken(TokenType type, Token *token, Scanner *scanner)
{
	char *text = substring(scanner->source, scanner->start, scanner->current - scanner->start);
	token->type = type;
	token->literal = text;
}

static char advance(Scanner *scanner)
{
	if (scanner->current >= scanner->source_length)
	{
		return '\0';
	}
	return scanner->source[scanner->current++];
}

static char peek(Scanner *scanner)
{
	if (isAtEnd(scanner))
		return '\0';
	return scanner->source[scanner->current];
}

static void number(Scanner *scanner, Token *token)
{
	while (isdigit(peek(scanner)))
		advance(scanner);
	token->type = TOKEN_NUMBER;
	token->literal = ALLOCATE_MEMORY(int, sizeof(int));

	char *text = substring(scanner->source, scanner->start, scanner->current - scanner->start);
	int number = atoi(text);

	*(int *)(token->literal) = number;
	free(text);
}

static void identifier(Token *token, Scanner *scanner)
{
	while (isalnum(peek(scanner)))
		advance(scanner);
	char *text = substring(scanner->source, scanner->start, scanner->current - scanner->start);
	TokenType type = findKeyword(text);
	if (type == TOKEN_NULL)
		type = TOKEN_IDENTIFIER;
	token->type = type;
	token->literal = text;
}

static void addNullToken(Token *token)
{
	token->type = TOKEN_NULL;
}

static void string(char end, Token *token, Scanner *scanner)
{
	while (peek(scanner) != end && !isAtEnd(scanner))
	{
		advance(scanner);
	}
	if (isAtEnd(scanner))
	{
		addNullToken(token);
		fprintf(stderr, "Error: Unterminated string\n");
		return;
	}
	advance(scanner);
	token->type = TOKEN_IDENTIFIER;
	char *text = substring(scanner->source, scanner->start + 1, scanner->current - scanner->start - 2);
	token->literal = text;
}

void scanToken(Scanner *scanner, Token *token)
{
	if (token->type != TOKEN_NUMBER && token->type != TOKEN_NULL)
	{
		free(token->literal);
		token->type = TOKEN_NULL;
		token->literal = NULL;
	}

	char c = advance(scanner);

	while (c == '\n' || c == '\r' || c == ' ' || c == '\t')
	{
		c = advance(scanner);
	}
	scanner->start = scanner->current - 1;

	switch (c)
	{
	case '\0':
		break;
	case ';':
		addToken(TOKEN_SEMICOLON, token, scanner);
		break;
	case ',':
		addToken(TOKEN_COMMA, token, scanner);
		break;
	case '*':
		addToken(TOKEN_STAR, token, scanner);
		break;
	case '+':
		addToken(TOKEN_PLUS, token, scanner);
		break;
	case '-':
		addToken(TOKEN_MINUS, token, scanner);
		break;
	case '(':
		addToken(TOKEN_LPAREN, token, scanner);
		break;
	case ')':
		addToken(TOKEN_RPAREN, token, scanner);
		break;
	case '!':
		if (match('=', scanner))
			addToken(TOKEN_NOT_EQUALS, token, scanner);
		else
		{
			fprintf(stderr, "Error: Invalid symbol\n");
			addNullToken(token);
		}
		break;
	case '>':
		addToken(match('=', scanner) ? TOKEN_GT_EQUALS : TOKEN_GT, token, scanner);
		break;
	case '<':
		addToken(match('=', scanner) ? TOKEN_LT_EQUALS : TOKEN_LT, token, scanner);
		break;
	case '=':
		if (peek(scanner) == '=')
		{
			fprintf(stderr, "Error: Invalid symbol\n");
			addNullToken(token);
		}
		else
		{
			addToken(TOKEN_EQUALS, token, scanner);
		}
		break;
	case '"':
		string('"', token, scanner);
		break;
	case '\'':
		string('\'', token, scanner);
		break;
	default:
		if (isdigit(c))
		{
			number(scanner, token);
		}
		else if (isalpha(c))
		{
			identifier(token, scanner);
		}
		else
		{
			char *format = "Unexpected character: %c";
			char message[25];
			sprintf(message, format, c);
			fprintf(stderr, "Error: %s\n", message);
			addNullToken(token);
		}
		break;
	}
}
