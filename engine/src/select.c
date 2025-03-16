#include <string.h>
#include <strings.h>
#include "parser.h"

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
	ASTNode *node = ALLOCATE_MEMORY(ASTNode, sizeof(ASTNode), cleanup);
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
	while (match(1, TOKEN_IDENTIFIER))
	{
		ASTNode *columnNode = createNode(COLUMN);
		columnNode->value.stringValue = strdup((char *)previous().literal); // handle memory fail (TODO)
		addChild(parent, columnNode);

		if (!match(1, TOKEN_COMMA))
			break;
	}
}

static void parseTable(ASTNode *parent)
{
	Token current = advance();
	if (current.type != TOKEN_IDENTIFIER)
	{
		printf("Error at tbale");
		return;
	}
	ASTNode *table = createNode(TABLE);
	table->value.stringValue = strdup((char *)current.literal);
	addChild(parent, table);
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
	ASTNode *condition = createNode(WHERE);
	ASTNode *comparison = createNode(COMPARISON);
	ASTNode *left = NULL;
	if (peek().type == TOKEN_NUMBER)
	{
		left = createNode(NUMBER);
		left->value.intValue = atoi((char *)peek().literal);
	}
	else if (peek().type == TOKEN_IDENTIFIER)
	{
		left = createNode(ID);
		left->value.stringValue = strdup((char *)peek().literal);
	}
	if (left == NULL)
	{
		// handle this
		return;
	}
	advance();
	Token op = peek();

	char *operator= getOp(op.type);
	if (operator== NULL)
	{
		// Handle error
		return;
	}
	comparison->comparisonOperator = strdup(operator);
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
		right->value.stringValue = strdup((char *)peek().literal);
	}
	if (left == NULL)
	{
		// handle this
		return;
	}
	comparison->left = left;
	comparison->right = right;
	addChild(condition, comparison);
	addChild(parent, condition);
	advance();
}

ASTNode *selectStatement()
{
	ASTNode *selectNode = createNode(SELECT);
	parseColumns(selectNode);
	consume(TOKEN_FROM, "Error: expected from statement.");
	parseTable(selectNode);

	if (match(1, TOKEN_WHERE))
		parseWhere(selectNode);

	consume(TOKEN_SEMICOLON, "Query must end with a semi colon.");

	return selectNode;
}
