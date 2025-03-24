#include "processor.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: db <filename>\n");
		fprintf(stderr, "Usage: db <filename>%s", argv[1]);
		exit(EXIT_FAILURE);
	}

	run();
	// FILE *file;

	// file = fopen(argv[1], "rb");
	// if (file == NULL)
	// {
	// 	addMasterPage(argv[1]);
	// }

	// void *page = allocatePage();
	// size_t bytes_read = fread(page, PAGE_SIZE, 1, file);
	// if (bytes_read < 1)
	// {
	// 	fprintf(stderr, "Could not find page");
	// 	exit(EXIT_FAILURE);
	// }
	// PageHeader *pageHeader = (PageHeader *)((uint8_t *)page + sizeof(DatabaseHeader));
	// fclose(file);
	// free(page);

	return 0;
}
