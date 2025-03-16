#ifndef _Lexer_H
#define _Lexer_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <stdlib.h>
#include "error.h"
#include "memory.h"

typedef enum
{
	// End of Input
	TOKEN_EOF, // End of file

	// Delimiters
	TOKEN_SEMICOLON, // ;
	TOKEN_COMMA,	 // ,
	TOKEN_LPAREN,	 // (
	TOKEN_RPAREN,	 // )

	// Keywords
	TOKEN_SELECT, // SELECT
	TOKEN_INSERT, // INSERT
	TOKEN_FROM,	  // FROM
	TOKEN_WHERE,  // WHERE
	TOKEN_VALUES, // VALUES
	TOKEN_INTO,	  // INTO

	// Operators
	TOKEN_EQUALS,	  // =
	TOKEN_NOT_EQUALS, // !=
	TOKEN_GT,		  // >
	TOKEN_GT_EQUALS,  // >=
	TOKEN_LT,		  // <
	TOKEN_LT_EQUALS,  // <=
	TOKEN_BANG,		  // !

	// Literals
	TOKEN_IDENTIFIER, // Column/Table names
	TOKEN_NUMBER,	  // Numeric values
	TOKEN_NULL,		  // NULL keyword

	// Special Characters
	TOKEN_STAR // *
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

bool initScanner(char *source);
TokenList scanTokens();
void freeTokens();
void printToken(Token token);
void printTokens();
#endif
