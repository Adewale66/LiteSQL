#include "lexer.h"
#include "parser.h"
#include "chunk.h"
#include <fcntl.h>
#include <unistd.h>
#include "btree.h"

int main()
{
	// int fd;
	// if (argc != 2)
	// {
	// 	fprintf(stderr, "Usage: db <filename>\n");
	// 	exit(EXIT_FAILURE);
	// }
	// fd = open(argv[1], O_CREAT | O_RDWR, 0644);
	// if (fd == -1)
	// {
	// 	fprintf(stderr, "Unable to create or read file\n");
	// 	exit(EXIT_FAILURE);
	// }
	// CREATE OR GET MASTER TABLE
	// LOAD MASTER TABLE INTO MEMORY
	// INIT VM

	BTree *root = NULL;
	int nums[] = {100, 50, 150, 25, 75, 125, 175, 10, 30, 60, 90, 110, 140, 160, 190};
	int len = 15;

	for (int i = 0; i < len; i++)
	{
		root = insert(root, NULL, nums[i], -1);
	}

	printBTree(root, 0);
	freeBTree(root);
}
