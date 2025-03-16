#include <string.h>
#include "parser.h"

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

ASTNode *selectStatement()
{
	ASTNode *selectNode = createNode(SELECT);
	parseColumns(selectNode);
	consume(TOKEN_FROM, "Error: expected from statement.");
	parseTable(selectNode);
	consume(TOKEN_SEMICOLON, "Query must end with a semi colon.");

	return selectNode;
}
