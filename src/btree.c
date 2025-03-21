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

static BTree *createNode(bool leaf)
{
	BTree *node = (BTree *)malloc(sizeof(BTree));
	node->leaf = leaf;
	node->keyCount = 0;
	memset(node->children, 0, sizeof(node->children));
	return node;
}

// needs fixing
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

static BTree *getPredecessor(BTree *node)
{

	if (node->leaf)
	{
		return node;
	}
	return getPredecessor(node->children[node->keyCount]);
}

static BTree *getSuccessor(BTree *node)
{

	if (node->leaf)
	{
		return node;
	}
	return getSuccessor(node->children[0]);
}

static BTree *balanceNode(BTree *node, BTree *parent, int index)
{
	int leftSibling = index - 1;
	int rightSibling = index + 1;

	if (leftSibling >= 0 && parent->children[leftSibling]->keyCount > MIN_KEYS)
	{

		BTree *lastChild = parent->children[leftSibling]->children[parent->children[leftSibling]->keyCount];
		int maxLeft = parent->children[leftSibling]->keys[parent->children[leftSibling]->keyCount - 1];
		parent->children[leftSibling]->keyCount--;
		int parentValue = parent->keys[index - 1];
		parent->keys[index - 1] = maxLeft;
		for (int i = node->keyCount; i > 0; i--)
		{
			node->keys[i] = node->keys[i + 1];
		}
		node->keys[0] = parentValue;
		node->keyCount++;

		for (int i = node->keyCount; i > 0; i--)
		{
			node->children[i] = node->children[i - 1];
		}
		node->children[0] = lastChild;
	}
	else if (rightSibling <= parent->keyCount && parent->children[rightSibling]->keyCount > MIN_KEYS)
	{

		int parentValue = parent->keys[index];
		BTree *firstChild = parent->children[rightSibling]->children[0];
		node->keys[node->keyCount++] = parentValue;
		int minRight = parent->children[rightSibling]->keys[0];
		parent->keys[index] = minRight;
		for (int i = 0; i < parent->children[rightSibling]->keyCount - 1; i++)
		{
			parent->children[rightSibling]->keys[i] = parent->children[rightSibling]->keys[i + 1];
		}
		parent->children[rightSibling]->keyCount--;
		node->children[node->keyCount] = firstChild;
		for (int i = 0; i <= parent->children[rightSibling]->keyCount; i++)
		{
			parent->children[rightSibling]->children[i] = parent->children[rightSibling]->children[i + 1];
		}
	}
	else
	{

		int parentValue;
		int parentCount = parent->keyCount;
		if (leftSibling >= 0)
		{

			parentValue = parent->keys[index - 1];
			parent->children[leftSibling]->keys[parent->children[leftSibling]->keyCount++] = parentValue;

			int childrenCount = parent->children[leftSibling]->keyCount;

			for (int i = 0; i < node->keyCount; i++)
			{
				parent->children[leftSibling]->keys[parent->children[leftSibling]->keyCount++] = node->keys[i];
			}

			for (int i = 0; i <= node->keyCount; i++)
			{

				parent->children[leftSibling]->children[childrenCount++] = node->children[i];
			}
			for (int i = index - 1; i < parent->keyCount - 1; i++)
			{
				parent->keys[i] = parent->keys[i + 1];
			}
			parent->keyCount--;
			free(parent->children[index]);
			for (int i = index; i < parentCount; i++)
			{

				parent->children[i] = parent->children[i + 1];
			}
			node = parent->children[index - 1];
		}
		else
		{

			int oldCount = node->keyCount;
			parentValue = parent->keys[index];
			node->keys[node->keyCount++] = parentValue;

			for (int i = 0; i < parent->children[rightSibling]->keyCount; i++)
			{
				node->keys[node->keyCount++] = parent->children[rightSibling]->keys[i];
			}

			for (int i = index; i < parent->keyCount - 1; i++)
			{
				parent->keys[i] = parent->keys[i + 1];
			}
			parent->keyCount--;

			int idx = 0;
			for (int i = oldCount + 1; idx <= parent->children[rightSibling]->keyCount; i++)
			{
				node->children[i] = parent->children[rightSibling]->children[idx];
				idx++;
			}
			free(parent->children[index + 1]);
			for (int i = index + 1; i < parentCount; i++)
			{
				parent->children[i] = parent->children[i + 1];
			}
		}
	}
	return node;
}

BTree *delete(BTree *node, BTree *parent, int index, int value)
{

	// Value not in tree
	if (node == NULL)
	{
		return NULL;
	}

	int l = binarySearch(0, node->keyCount, value, node);
	if (l >= node->keyCount || node->keys[l] != value)
	{
		node->children[l] = delete (node->children[l], node, l, value);
		// parent check here
		if (node->keyCount < MIN_KEYS && parent != NULL)
		{
			node = balanceNode(node, parent, index);
		}
		else
		{
			if (node->keyCount == 0)
			{
				BTree *root = node->children[l];
				free(node);
				return root;
			}
		}
		return node;
	}

	if (node->leaf)
	{
		// If leaf contains more than min no of keys or is root
		if (node->keyCount > MIN_KEYS || parent == NULL)
		{

			for (int i = l; i < node->keyCount - 1; i++)
			{
				node->keys[i] = node->keys[i + 1];
			}
			node->keyCount--;
			return node;
		}
		else
		{

			// Check siblings if a key can be borrowed
			int leftIdx = index - 1;
			int rightIdx = index + 1;

			if (leftIdx >= 0 && parent->children[leftIdx]->keyCount > MIN_KEYS)
			{
				int maxLeft = parent->children[leftIdx]->keys[parent->children[leftIdx]->keyCount - 1];
				parent->children[leftIdx]->keyCount--;
				int parentValue = parent->keys[index - 1];
				parent->keys[index - 1] = maxLeft;
				for (int i = l; i > 0; i--)
				{
					node->keys[i] = node->keys[i - 1];
				}
				node->keys[0] = parentValue;
			}
			else if (rightIdx <= parent->keyCount && parent->children[rightIdx]->keyCount > MIN_KEYS)
			{

				int minRight = parent->children[rightIdx]->keys[0];

				for (int i = 0; i < parent->children[rightIdx]->keyCount - 1; i++)
				{
					parent->children[rightIdx]->keys[i] = parent->children[rightIdx]->keys[i + 1];
				}

				parent->children[rightIdx]->keyCount--;
				int parentValue = parent->keys[index];
				parent->keys[index] = minRight;

				for (int i = l; i < node->keyCount - 1; i++)
				{
					node->keys[i] = node->keys[i + 1];
				}
				node->keys[node->keyCount - 1] = parentValue;
			}
			else
			{

				// Merge with one of the it's siblings
				int parentValue = 0;
				int parentCount = parent->keyCount;
				if (leftIdx >= 0)
				{
					parentValue = parent->keys[index - 1];
					// merge left node and current node

					parent->children[leftIdx]->keys[parent->children[leftIdx]->keyCount++] = parentValue;

					for (int i = 0; i < node->keyCount; i++)
					{
						parent->children[leftIdx]->keys[parent->children[leftIdx]->keyCount++] = node->keys[i];
					}

					for (int i = index - 1; i < parent->keyCount - 1; i++)
					{
						parent->keys[i] = parent->keys[i + 1];
					}
					parent->keyCount--;
					int nl = binarySearch(0, parent->children[leftIdx]->keyCount, value, parent->children[leftIdx]);

					for (int i = nl; i < parent->children[leftIdx]->keyCount - 1; i++)
					{
						parent->children[leftIdx]->keys[i] = parent->children[leftIdx]->keys[i + 1];
					}
					parent->children[leftIdx]->keyCount--;

					free(parent->children[index]);
					for (int i = index; i < parentCount; i++)
					{
						parent->children[i] = parent->children[i + 1];
					}
					node = parent->children[index];
				}
				else if (rightIdx <= parent->keyCount)
				{

					parentValue = parent->keys[index];

					node->keys[node->keyCount++] = parentValue;
					for (int i = index; i < parent->keyCount - 1; i++)
					{
						parent->keys[i] = parent->keys[i + 1];
					}
					parent->keyCount--;

					for (int i = 0; i < parent->children[rightIdx]->keyCount; i++)
					{
						node->keys[node->keyCount++] = parent->children[rightIdx]->keys[i];
					}

					int nl = binarySearch(0, node->keyCount, value, node);

					for (int i = nl; i < node->keyCount - 1; i++)
					{
						node->keys[i] = node->keys[i + 1];
					}

					node->keyCount--;
					free(parent->children[index + 1]);
					for (int i = index + 1; i < parentCount; i++)
					{
						parent->children[i] = parent->children[i + 1];
					}
				}
			}
		}
	}
	else
	{
		// internal node

		// inorder predecessor
		BTree *predecessor = getPredecessor(node->children[l]);
		if (predecessor->keyCount > MIN_KEYS)
		{
			node->keys[l] = predecessor->keys[predecessor->keyCount - 1];
			predecessor->keyCount--;
			return node;
		}

		// inorder successor
		BTree *successor = getSuccessor(node->children[l + 1]);
		if (successor->keyCount > MIN_KEYS)
		{
			node->keys[l] = successor->keys[0];
			for (int i = 0; i < successor->keyCount - 1; i++)
			{
				successor->keys[i] = successor->keys[i + 1];
			}
			successor->keyCount--;
		}

		else
		{

			node->keys[l] = predecessor->keys[predecessor->keyCount - 1];
			node->children[l] = delete (node->children[l], node, l, node->keys[l]);

			if (node->keyCount == 0 && parent == NULL) // root
			{
				BTree *root = node->children[l];
				free(node);
				return root;
			}
			if (node->keyCount < MIN_KEYS && parent != NULL)
			{
				node = balanceNode(node, parent, index);
			}
		}
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
