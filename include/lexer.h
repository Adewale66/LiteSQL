#ifndef __Lexer_H
#define __Lexer_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <stdlib.h>

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
	TOKEN_CREATE, // CREATE
	TOKEN_DELETE, // CREATE
	TOKEN_UPDATE, // CREATE
	TOKEN_FROM,	  // FROM
	TOKEN_WHERE,  // WHERE
	TOKEN_VALUES, // VALUES
	TOKEN_INTO,	  // INTO
	TOKEN_SET,	  // STRING
	TOKEN_TABLE,  // CREATE
	TOKEN_INT,	  // INT
	TOKEN_STRING, // STRING

	// Operators
	TOKEN_EQUALS,	  // =
	TOKEN_NOT_EQUALS, // !=
	TOKEN_GT,		  // >
	TOKEN_GT_EQUALS,  // >=
	TOKEN_LT,		  // <
	TOKEN_LT_EQUALS,  // <=
	TOKEN_PLUS,		  // +
	TOKEN_MINUS,	  // -
	TOKEN_DIVIDE,	  // /

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
	size_t current;
	size_t source_length;

} Scanner;

void initScanner(Scanner *scanner, char *source, size_t length);
void scanToken(Scanner *scanner, Token *token);
#endif
