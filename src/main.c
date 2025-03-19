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

	BTree *root = insert(NULL, NULL, 50, -1);
	root = insert(root, NULL, 30, -1);
	root = insert(root, NULL, 70, -1);
	// root = insert(root, NULL, 20, -1);
	// root = insert(root, NULL, 40, -1);
	// root = insert(root, NULL, 60, -1);
	// root = insert(root, NULL, 80, -1);
	// root = insert(root, NULL, 10, -1);
	// root = insert(root, NULL, 90, -1);
	// root = insert(root, NULL, 25, -1);
	root = delete (root, NULL, -1, 70);
	// printBTree(root, 0);

	freeBTree(root);
}
