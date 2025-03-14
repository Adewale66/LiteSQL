#ifndef _Lexer_H
#define _Lexer_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <stdlib.h>
#include "memory.h"

typedef enum
{
	TOKEN_EOF,
	TOKEN_SEMICOLON,
	TOKEN_SELECT,
	TOKEN_INSERT,
	TOKEN_FROM,
	TOKEN_COMMA,
	TOKEN_STAR,
	TOKEN_IDENTIFIER,
	TOKEN_NULL,
	TOKEN_NUMBER,
	TOKEN_VALUES,
	TOKEN_INTO,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_STRING
} TokenType;

typedef struct
{
	TokenType type;
	void *literal;
} Token;

typedef struct
{
	int size;
	int capacity;
	Token *tokens;
} TokenList;

typedef struct
{
	char *source;
	int start;
	int current;
	uint8_t source_length;
	TokenList tokenList;

} Scanner;

void initScanner(char *source);
TokenList scanTokens();
void freeTokens();
void printTokens();
#endif
