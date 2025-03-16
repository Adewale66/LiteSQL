#include "parser.h"

static Parser parser;

void freeNode(ASTNode *node)
{
	if (node == NULL)
		return;
	if (node->children != NULL)
	{
		for (int i = 0; i < node->childCount; i++)
		{
			freeNode(node->children[i]);
		}
		free(node->children);
	}
	if (node->value.stringValue != NULL)
	{
		free(node->value.stringValue);
	}
	freeNode(node->left);
	freeNode(node->right);
	free(node);
}

Token previous()
{
	return parser.tokenlist.tokens[parser.current - 1];
}

Token peek()
{
	return parser.tokenlist.tokens[parser.current];
}

bool isAtEnd()
{
	return peek().type == TOKEN_EOF;
}

Token advance()
{
	if (!isAtEnd())
		parser.current++;
	return previous();
}

bool check(TokenType type)
{
	if (isAtEnd())
		return false;
	return peek().type == type;
}

bool match(int n, ...)
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

void consume(TokenType type, char *error_message, bool *hadError)
{
	if (*hadError == true)
		return;
	if (check(type))
	{
		advance();
		return;
	}
	error(error_message);
	*hadError = true;
	return;
}

static ASTNode *parse()
{
	if (match(1, TOKEN_SELECT))
	{
		parser.root = selectStatement();
		freeTokens();
	}
	else
		error("Unsupported query type.");
	return parser.root;
}

ASTNode *initParser(TokenList tokens)
{
	parser.tokenlist = tokens;
	parser.current = 0;
	parser.root = NULL;
	return parse();
}

void memoryCleanup()
{
	freeNode(parser.root);
}

void printAST(ASTNode *node, int indent)
{
	for (int i = 0; i < indent; i++)
	{
		printf("  ");
	}

	switch (node->type)
	{
	case SELECT:
		printf("SELECT\n");
		break;
	case COLUMN:
	case ID:
		printf("COLUMN: %s\n", node->value.stringValue);
		break;
	case TABLE:
		printf("TABLE: %s\n", node->value.stringValue);
		break;
	case WHERE:
		printf("WHERE\n");
		break;
	case COMPARISON:
		printf("COMPARISON: %s\n", node->comparisonOperator);
		break;
	case NUMBER:
		printf("LITERAL_INT: %d\n", node->value.intValue);
		break;
	}

	if (node->left != NULL)
	{
		printAST(node->left, indent + 1);
	}
	if (node->right != NULL)
	{
		printAST(node->right, indent + 1);
	}
	for (int i = 0; i < node->childCount; i++)
	{
		printAST(node->children[i], indent + 1);
	}
}
