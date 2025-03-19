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

static ASTNode *createNode(NodeType type, ASTNode *parent)
{
	ASTNode *node = ALLOCATE_MEMORY_WITH_ARGS(ASTNode, sizeof(ASTNode), freeNode, parent);
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

static char *getOp(TokenType type)
{
	for (int i = 0; i < opCount; i++)
	{
		if (operators[i].type == type)
			return operators[i].literal;
	}
	return NULL;
}
static bool parseWhere(ASTNode *parent)
{

	ASTNode *condition = createNode(WHERE, parent);
	ASTNode *comparison = createNode(COMPARISON, parent);

	addChild(condition, comparison);
	addChild(parent, condition);

	ASTNode *left = NULL;
	if (peek().type == TOKEN_NUMBER)
	{
		left = createNode(NUMBER, parent);
		left->value.intValue = *((int *)peek().literal);
	}
	else if (peek().type == TOKEN_IDENTIFIER)
	{
		left = createNode(ID, parent);
		left->value.stringValue = COPY_STRING((char *)peek().literal, freeNode, parent);
	}
	else
	{
		fprintf(stderr, "Error: Unexpected identifier\n");
		freeNode(parent);
		return false;
	}

	comparison->left = left;

	advance();
	Token op = peek();

	char *operator= getOp(op.type);
	if (operator== NULL)
	{
		fprintf(stderr, "Error: Expected 'operator'.\n");
		freeNode(parent);
		return false;
	}
	comparison->comparisonOperator = operator;

	advance();

	ASTNode *right = NULL;
	if (peek().type == TOKEN_NUMBER)
	{
		right = createNode(NUMBER, parent);
		right->value.intValue = *((int *)peek().literal);
	}
	else if (peek().type == TOKEN_IDENTIFIER)
	{
		right = createNode(ID, parent);
		right->value.stringValue = COPY_STRING((char *)peek().literal, freeNode, parent);
	}
	else
	{
		fprintf(stderr, "Error: Unexpecteda identifier\n");
		freeNode(parent);
		return false;
	}
	comparison->right = right;

	advance();
	return true;
}

static ASTNode *checkClause(ASTNode *parent)
{
	bool success = false;
	if (match(1, TOKEN_WHERE))
		success = parseWhere(parent);

	if (success)
	{
		if (!consume(TOKEN_SEMICOLON, "Expected ';' after query."))
		{
			freeNode(parent);
			return NULL;
		}
		return parent;
	}
	return NULL;
}

static ASTNode *parseTable(ASTNode *parent)
{

	Token current = advance();
	if (current.type != TOKEN_IDENTIFIER)
	{
		fprintf(stderr, "Error: Expected table name.\n");
		freeNode(parent);
		return NULL;
	}
	ASTNode *table = createNode(TABLE, parent);

	addChild(parent, table);

	table->value.stringValue = COPY_STRING((char *)current.literal, freeNode, parent);

	return checkClause(parent);
}

static ASTNode *parseColumns(ASTNode *parent)
{
	while (match(1, TOKEN_IDENTIFIER))
	{
		ASTNode *columnNode = createNode(COLUMN, parent);
		addChild(parent, columnNode);
		columnNode->value.stringValue = COPY_STRING((char *)previous().literal, freeNode, parent);

		if (match(1, TOKEN_COMMA))
		{
			if (peek().type != TOKEN_IDENTIFIER)
			{
				fprintf(stderr, "Error: Expected identifier after ','.\n");
				freeNode(parent);
				return NULL;
			}
		}
		else
			break;
	}

	if (!consume(TOKEN_FROM, "Expected \"FROM\" after table name."))
	{
		freeNode(parent);
		return NULL;
	};
	return parseTable(parent);
}

ASTNode *selectStatement()
{
	ASTNode *selectNode = createNode(SELECT, NULL);
	return parseColumns(selectNode);
}
