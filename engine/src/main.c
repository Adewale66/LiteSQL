#include "lexer.h"
#include "parser.h"

int main()
{
	char *statement = "SELECT id,name,state,place from somewhere where id > 1;";
	initScanner(statement);
	TokenList tokens = scanTokens();
	if (tokens.size != 0)
	{
		ASTNode *root = initParser(tokens);
		if (root != NULL)
		{
			printAST(root, 0);
			freeNode(root);
		}
	}
}
