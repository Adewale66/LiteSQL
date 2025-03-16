#include "lexer.h"
#include "parser.h"

int main()
{
	char *statement = "SELECT id,name,state,place FROM unidentiferedtbalename;";
	initScanner(statement);
	TokenList tokens = scanTokens();
	if (tokens.size != 0)
	{
		ASTNode *root = intiParser(tokens);
		printAST(root, 0);
		freeNode(root);
	}
}
