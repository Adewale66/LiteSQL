#include "lexer.h"
#include "parser.h"

int main()
{
	char *statement = "SELECT id, name FROM table;";
	initScanner(statement);
	TokenList tokens = scanTokens();
	intiParser(tokens);

	// printTokens();
	freeTokens();
}
