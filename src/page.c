#include "page.h"

void *allocatePage()
{
	void *page = calloc(1, PAGE_SIZE);
	if (page == NULL)
	{
		fprintf(stderr, "Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	return page;
}

void addMasterPage(char *filename)
{
	FILE *file = fopen(filename, "wb");
	void *page = allocatePage();
	DatabaseHeader *db_header = (DatabaseHeader *)page;
	db_header->page_count = 1;
	db_header->page_size = PAGE_SIZE;
	strncpy(db_header->header_string, "LiteSQL format v1\0", sizeof(db_header->header_string));

	PageHeader *pageHeader = (PageHeader *)((uint8_t *)page + sizeof(DatabaseHeader));
	pageHeader->id = 1;
	pageHeader->num_cells = 0;
	pageHeader->type = LEAF;
	pageHeader->free_start = sizeof(DatabaseHeader) + sizeof(PageHeader);
	pageHeader->free_end = PAGE_SIZE;
	pageHeader->total_free = pageHeader->free_end - pageHeader->free_start;

	fwrite(page, PAGE_SIZE, 1, file);
	fclose(file);
	free(page);
}
