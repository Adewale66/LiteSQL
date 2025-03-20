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
	int nums[] = {50, 30, 70, 10, 40, 60, 80, 5, 15, 35, 45, 55, 65, 75, 85};
	int len = 15;

	for (int i = 0; i < len; i++)
	{
		root = insert(root, NULL, nums[i], -1);
	}

	root = delete (root, NULL, -1, 50);
	// root = delete (root, NULL, -1, 15);
	// root = delete (root, NULL, -1, 35);
	// root = delete (root, NULL, -1, 45);
	// root = delete (root, NULL, -1, 55);
	// root = delete (root, NULL, -1, 65);
	// root = delete (root, NULL, -1, 75);
	// root = delete (root, NULL, -1, 85);

	printBTree(root, 0);
	freeBTree(root);
}
