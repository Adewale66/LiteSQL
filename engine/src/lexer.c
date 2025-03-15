#include "lexer.h"

static Scanner scanner;

void freeTokens()
{
	for (int i = 0; i < scanner.tokenList.size; i++)
	{
		if (scanner.tokenList.tokens[i].literal != NULL)
			free(scanner.tokenList.tokens[i].literal);
	}
	free(scanner.tokenList.tokens);
}
static char *substring(char *source, int start, int length)
{
	char *sub = (char *)malloc(length + 1);
	if (!sub)
	{

		fprintf(stderr, "Could not malloc\n");
		freeTokens();
		exit(2);
	}

	memcpy(sub, source + start, length);
	sub[length] = '\0';
	return sub;
}

static TokenType findType(char *text)
{
	if (strcasecmp("select", text) == 0)
	{
		return TOKEN_SELECT;
	}
	if (strcasecmp("insert", text) == 0)
	{
		return TOKEN_INSERT;
	}
	if (strcasecmp("from", text) == 0)
	{
		return TOKEN_FROM;
	}
	if (strcasecmp("values", text) == 0)
	{
		return TOKEN_VALUES;
	}
	if (strcasecmp("into", text) == 0)
	{
		return TOKEN_INTO;
	}
	return TOKEN_NULL;
}

void initScanner(char *source)
{

	if (source == NULL)
	{
		fprintf(stderr, "Source can not be null\n");
		return;
	}

	scanner.source = source;
	scanner.start = 0;
	scanner.source_length = strlen(source);
	scanner.current = 0;

	TokenList tokens;
	tokens.capacity = 0;
	tokens.size = 0;
	tokens.tokens = NULL;

	scanner.tokenList = tokens;
}

static void addToArray(TokenList *list, Token token)
{
	if (list->capacity < list->size + 1)
	{
		int oldCapacity = list->capacity;
		list->capacity = GROW_CAPACITY(oldCapacity);
		list->tokens = GROW_ARRAY(Token, list->tokens, oldCapacity, list->capacity);
	}

	list->tokens[list->size] = token;
	list->size++;
}

static void addToken(TokenType type)
{
	char *text = substring(scanner.source, scanner.start, scanner.current - scanner.start);
	if (text == NULL)
	{
		fprintf(stderr, "Could not create substring\n");
		return;
	}

	Token token;
	token.type = type;
	token.literal = text;
	addToArray(&scanner.tokenList, token);
}

static bool isAtEnd()
{
	return scanner.current >= scanner.source_length;
}

static char advance()
{
	return scanner.source[scanner.current++];
}

static char peek()
{
	if (isAtEnd())
		return '\0';
	return scanner.source[scanner.current];
}

static char peekNext()
{
	if (scanner.current + 1 >= scanner.source_length)
		return '\0';
	return scanner.source[scanner.current + 1];
}
static void number()
{
	while (isdigit(peek()))
		advance();
	if (peek() == '.' && isdigit(peekNext()))
	{
		advance();
		while (isdigit(peek()))
			advance();
	}
	Token token;
	token.type = TOKEN_NUMBER;
	char *text = substring(scanner.source, scanner.start, scanner.current - scanner.start);
	double number = strtod(text, NULL);

	token.literal = malloc(sizeof(double));
	if (token.literal == NULL)
	{
		fprintf(stderr, "Could not malloc\n");
		freeTokens();
		exit(2);
	}
	*(double *)token.literal = number;
	addToArray(&scanner.tokenList, token);
	free(text);
}

static void identifier()
{
	while (isalnum(peek()))
		advance();
	char *text = substring(scanner.source, scanner.start, scanner.current - scanner.start);
	if (text == NULL)
	{
		fprintf(stderr, "Error at identifer\n");
		return;
	}
	TokenType type = findType(text);
	if (type == TOKEN_NULL)
		type = TOKEN_IDENTIFIER;
	free(text);
	addToken(type);
}

static void string()
{
	while (peek() != '"' && !isAtEnd())
	{
		advance();
	}
	if (isAtEnd())
	{
		fprintf(stderr, "Unterminated string\n");
		return;
	}
	advance();
	Token token;
	token.type = TOKEN_STRING;
	char *text = substring(scanner.source, scanner.start + 1, scanner.current - scanner.start - 2);
	token.literal = text;
	addToArray(&scanner.tokenList, token);
}

static void scanToken()
{
	char c = advance();
	switch (c)
	{
	case ';':
		addToken(TOKEN_SEMICOLON);
		break;
	case ',':
		addToken(TOKEN_COMMA);
		break;
	case '*':
		addToken(TOKEN_STAR);
		break;
	case '(':
		addToken(TOKEN_LPAREN);
		break;
	case ')':
		addToken(TOKEN_RPAREN);
		break;
	case ' ':
	case '\r':
	case '\t':
	case '\n':
		// Ignore whitespace.
		break;
	case '"':
		string();
		break;
	default:
		if (isdigit(c))
		{
			number();
		}
		else if (isalpha(c))
		{
			identifier();
		}
		else
		{
			fprintf(stderr, "Unexpected character %c", c);
		}
		break;
	}
}

TokenList scanTokens()
{
	while (!isAtEnd())
	{
		scanner.start = scanner.current;
		scanToken();
	}
	Token token;
	token.type = TOKEN_EOF;
	token.literal = NULL;
	addToArray(&scanner.tokenList, token);
	return scanner.tokenList;
}

void printToken(Token token)
{

	switch (token.type)
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
		printf("IDENTIFER: %s", (char *)token.literal);
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
		printf("NUMBER: %f", *((double *)token.literal));
		break;
	case TOKEN_STRING:
		printf("STRING: %s", (char *)token.literal);
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
		printf("Token not recognized %s", (char *)token.literal);
		break;
	}
}

void printTokens()
{
	TokenList list = scanner.tokenList;
	printf("%d\n", list.size);
	for (int i = 0; i < list.size; i++)
	{
		printToken(list.tokens[i]);
		if (i != list.size - 1)
			printf(", ");
	}
	printf("\n");
}
