#include <strings.h>
#include "parser.h"

static bool hadError;

static struct
{
	TokenType type;
	char *literal;
} operators[] = {
	{TOKEN_EQUALS, "="},
	{TOKEN_GT, ">"},
	{TOKEN_GT_EQUALS, ">="},
	{TOKEN_LT, "<"},
	{TOKEN_LT_EQUALS, "<="},
	{TOKEN_NOT_EQUALS, "!="}};

static int opCount = 6;

static ASTNode *createNode(NodeType type)
{
	ASTNode *node = ALLOCATE_MEMORY(ASTNode, sizeof(ASTNode), memoryCleanup);
	node->type = type;
	node->childCount = 0;
	node->children = NULL;
	node->value.stringValue = NULL;
	node->comparisonOperator = NULL;
	node->left = NULL;
	node->right = NULL;

	return node;
}

static void addChild(ASTNode *parent, ASTNode *child)
{
	parent->childCount++;
	parent->children = GROW_ARRAY(ASTNode *, parent->children, parent->childCount * sizeof(ASTNode *));
	parent->children[parent->childCount - 1] = child;
}

static void parseColumns(ASTNode *parent)
{
	if (hadError)
		return;
	while (match(1, TOKEN_IDENTIFIER))
	{
		ASTNode *columnNode = createNode(COLUMN);
		addChild(parent, columnNode);
		columnNode->value.stringValue = COPY_STRING((char *)previous().literal, memoryCleanup);

		if (match(1, TOKEN_COMMA))
		{
			if (peek().type != TOKEN_IDENTIFIER)
			{
				error("Expected identifier after ','.");
				hadError = true;
				break;
			}
		}
		else
			break;
	}
}

static void parseTable(ASTNode *parent)
{
	if (hadError)
		return;
	Token current = advance();
	if (current.type != TOKEN_IDENTIFIER)
	{
		hadError = true;
		error("Expected table name.");
		return;
	}
	ASTNode *table = createNode(TABLE);

	addChild(parent, table);

	table->value.stringValue = COPY_STRING((char *)current.literal, memoryCleanup);
}

static char *getOp(TokenType type)
{
	for (int i = 0; i < opCount; i++)
	{
		if (operators[i].type == type)
			return operators[i].literal;
	}
	return NULL;
}
static void parseWhere(ASTNode *parent)
{
	if (hadError)
		return;
	ASTNode *condition = createNode(WHERE);
	ASTNode *comparison = createNode(COMPARISON);

	addChild(condition, comparison);
	addChild(parent, condition);

	ASTNode *left = NULL;
	if (peek().type == TOKEN_NUMBER)
	{
		left = createNode(NUMBER);
		left->value.intValue = atoi((char *)peek().literal);
	}
	else if (peek().type == TOKEN_IDENTIFIER)
	{
		left = createNode(ID);
		left->value.stringValue = COPY_STRING((char *)peek().literal, memoryCleanup);
	}
	else
	{
		error("Unexpected identifier");
		hadError = true;
		return;
	}

	comparison->left = left;

	advance();
	Token op = peek();

	char *operator= getOp(op.type);
	if (operator== NULL)
	{
		error("Expected 'operator'");
		hadError = true;
		return;
	}
	comparison->comparisonOperator = operator;

	advance();

	ASTNode *right = NULL;
	if (peek().type == TOKEN_NUMBER)
	{
		right = createNode(NUMBER);
		right->value.intValue = atoi((char *)peek().literal);
	}
	else if (peek().type == TOKEN_IDENTIFIER)
	{
		right = createNode(ID);
		right->value.stringValue = COPY_STRING((char *)peek().literal, memoryCleanup);
	}
	else
	{
		error("Unexpected identifier");
		hadError = true;
		return;
	}
	comparison->right = right;

	advance();
}

ASTNode *selectStatement()
{
	hadError = false;
	ASTNode *selectNode = createNode(SELECT);
	parseColumns(selectNode);
	consume(TOKEN_FROM, "Expected \"FROM\" after table name.", &hadError);
	parseTable(selectNode);

	if (match(1, TOKEN_WHERE))
		parseWhere(selectNode);

	consume(TOKEN_SEMICOLON, "Expected ';' after query.", &hadError);

	if (hadError)
	{
		freeNode(selectNode);
		return NULL;
	}
	return selectNode;
}
