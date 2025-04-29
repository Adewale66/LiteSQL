#ifndef __EXPR_H
#define __EXPR_H

typedef struct Expression Expression;
typedef struct Binary Binary;
typedef struct Grouping Grouping;
typedef struct Unary Unary;
typedef struct Literal Literal;
typedef enum BinaryOperator BinaryOperator;
typedef enum ColumnType ColumnType;

typedef enum
{
	BINARY,
	GROUPING,
	UNARY,
	LITERAL,
} ExpressionType;

enum ColumnType
{
	INT,
	STRING
};

enum BinaryOperator
{

	EQUALS,
	GREATER_THAN,
	GREATER_THAN_OR_EQUALS,
	LESS_THAN,
	LESS_THAN_OR_EQUALS,
	NOT_EQUALS,
	BAD_OP,
	PLUS,
	MINUS
};

struct Expression
{
	ExpressionType type;
	union
	{
		Binary *binary;
		Grouping *group;
		Unary *unary;
		Literal *literal;
	};
};

struct Binary
{
	Expression left;
	Expression right;
	BinaryOperator op;
};

struct Grouping
{
	Expression expr;
};

struct Unary
{
	BinaryOperator op;
	Expression expr;
};

struct Literal
{
	void *value;
	ColumnType type;
};

#endif
