#ifndef __BTREE_H
#define __BTREE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_DEGREE 3

typedef struct BTree BTree;

struct BTree
{

	int keyCount;
	int keys[MAX_DEGREE];
	BTree *children[MAX_DEGREE + 1];
	bool leaf;
};

BTree *createNode(bool leaf);
BTree *insert(BTree *node, BTree *parent, int key, int index);
void printBTree(BTree *root, int depth);
void freeBTree(BTree *root);
#endif
