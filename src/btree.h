#ifndef __BTREE_H
#define __BTREE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_DEGREE 4
#define MIN_KEYS ((int)ceil((double)MAX_DEGREE / 2) - 1)

typedef struct BTree BTree;

struct BTree
{

	int keyCount;
	int keys[MAX_DEGREE];
	BTree *children[MAX_DEGREE + 1];
	bool leaf;
};

BTree *insert(BTree *node, BTree *parent, int key, int index);
BTree *delete(BTree *node, BTree *parent, int index, int value);

void printBTree(BTree *root, int depth);
void freeBTree(BTree *root);

#endif
