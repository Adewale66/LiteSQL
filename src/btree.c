#include "btree.h"

static int binarySearch(int l, int r, int value, BTree *node)
{
	while (l < r)
	{
		int mid = l + (r - l) / 2;
		if (node->keys[mid] < value)
		{
			l = mid + 1;
		}
		else
		{
			r = mid;
		}
	}
	return l;
}

static BTree *balance(BTree *node, BTree *parent, int passedIndex)
{
	int mid = node->keyCount / 2;
	BTree *current;
	BTree *right;
	if (parent == NULL) // root node
	{
		current = createNode(false);
		current->keys[current->keyCount++] = node->keys[mid];
		current->children[0] = node;
		right = createNode(node->leaf);
		current->children[1] = right;
	}
	else
	{
		current = node;
		if (passedIndex > parent->keyCount)
		{
			parent->keys[parent->keyCount++] = node->keys[mid];
		}
		else
		{
			for (int i = parent->keyCount; i > passedIndex; i--)
			{
				parent->keys[i] = parent->keys[i - 1];
			}
			parent->keyCount++;
			parent->keys[passedIndex] = node->keys[mid];
		}
		right = createNode(node->leaf);

		for (int i = parent->keyCount + 1; i > passedIndex + 1; i--)
		{

			parent->children[i] = parent->children[i - 1];
		}
		parent->children[passedIndex + 1] = right;
	}

	for (int i = mid + 1; i < node->keyCount; i++)
	{
		right->keys[right->keyCount++] = node->keys[i];
	}
	int count = node->keyCount;
	for (int i = mid; i < count; i++)
	{
		node->keys[i] = 0;
		node->keyCount--;
	}

	if (!node->leaf)
	{
		int rightIdx = 0;
		for (int i = mid + 1; i < MAX_DEGREE + 1; i++)
		{
			right->children[rightIdx] = node->children[i];
			rightIdx++;
		}
		for (int i = mid + 1; i < MAX_DEGREE + 1; i++)
		{
			node->children[i] = NULL;
		}
	}
	return current;
}

BTree *createNode(bool leaf)
{
	BTree *node = (BTree *)malloc(sizeof(BTree));
	node->leaf = leaf;
	node->keyCount = 0;
	memset(node->children, 0, sizeof(node->children));
	return node;
}

BTree *insert(BTree *node, BTree *parent, int value, int index)
{
	if (node == NULL)
	{
		BTree *newNode = createNode(true);
		newNode->keys[newNode->keyCount++] = value;
		return newNode;
	}
	if (node->leaf)
	{
		int l = binarySearch(0, node->keyCount, value, node);
		if (l >= node->keyCount)
		{
			node->keys[l] = value;
		}
		else
		{
			for (int i = node->keyCount; i > l; i--)
			{
				node->keys[i] = node->keys[i - 1];
			}
			node->keys[l] = value;
		}
		node->keyCount++;
		if (node->keyCount == MAX_DEGREE)
		{
			node = balance(node, parent, index);
		}
		return node;
	}

	int i = binarySearch(0, node->keyCount, value, node);
	node->children[i] = insert(node->children[i], node, value, i);
	if (node->keyCount == MAX_DEGREE)
	{
		node = balance(node, parent, index);
	}
	return node;
}

void printBTree(BTree *root, int level)
{
	if (root == NULL)
	{
		return;
	}

	for (int i = 0; i < level; ++i)
	{
		printf("  ");
	}

	printf("[");
	for (int i = 0; i < root->keyCount; ++i)
	{
		printf("%d", root->keys[i]);
		if (i < root->keyCount - 1)
		{
			printf(", ");
		}
	}
	printf("]");

	if (root->leaf)
	{
		printf(" (Leaf)\n");
	}
	else
	{
		printf("\n");
		for (int i = 0; i <= root->keyCount; ++i)
		{
			printBTree(root->children[i], level + 1);
		}
	}
}

void freeBTree(BTree *node)
{

	if (node == NULL)
		return;

	int count = node->keyCount;
	for (int i = 0; i <= count; i++)
	{
		freeBTree(node->children[i]);
	}
	node->keyCount = 0;
	free(node);
}
