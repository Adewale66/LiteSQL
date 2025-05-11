#include "../../include/pager.h"

static uint32_t generate_page_id(void)
{
	static uint32_t next_id = 1;
	return next_id++;
}

void *initSchemaPage(const char *filename)
{
	FILE *file = NULL;
	int temp_fd = -1;
	void *page = allocate_page(PAGE_SIZE);
	char temp_name[] = "/tmp/litesql-temp-XXXXXX";

	if (filename == NULL)
	{
		temp_fd = mkstemp(temp_name);
		if (temp_fd == -1)
		{
			free(page);
			perror("Failed to create temp file");
			exit(EXIT_FAILURE);
		}
		file = fdopen(temp_fd, "w+b");
		filename = temp_name;
	}
	else
	{
		file = fopen(filename, "r+b");
		if (file)
		{
			if (fread(page, PAGE_SIZE, 1, file) != 1)
			{
				perror("Could not read schema page");
				fclose(file);
				free(page);
				exit(EXIT_FAILURE);
			}
			fclose(file);
			return (page);
		}
		file = fopen(filename, "w+b");
	}

	if (!file)
	{
		perror("Failed to open file for writing");
		free(page);
		if (temp_fd != -1)
			close(temp_fd);
		exit(EXIT_FAILURE);
	}

	DatabaseHeader *db_header = (DatabaseHeader *)page;
	memset(db_header, 0, sizeof(DatabaseHeader));

	db_header->id = generate_page_id();
	snprintf(db_header->header_string, sizeof(db_header->header_string), "LiteSQL format v1");
	strncpy(db_header->filename, filename, sizeof(db_header->filename) - 1);

	db_header->page_size = PAGE_SIZE;
	db_header->page_count = 1;
	db_header->type = LEAF;
	db_header->free_start = sizeof(DatabaseHeader);
	db_header->free_end = PAGE_SIZE;
	db_header->total_free = db_header->free_end - db_header->free_start;
	db_header->num_rows = 0;
	db_header->rightmost_pointer = 0;
	db_header->flags = 0;

	fwrite(page, PAGE_SIZE, 1, file);
	fclose(file);
	return page;
}
