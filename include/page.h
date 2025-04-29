#ifndef __PAGES_H
#define __PAGES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define PAGE_SIZE 4096

typedef struct Cell Cell;
typedef enum PageType PageType;
typedef struct DatabaseHeader DatabaseHeader;
typedef struct PageHeader PageHeader;
typedef struct CellPointer CellPointer;

#define CELLPOINTER_OFFSET_TO_IDX(offset) \
	((offset - sizeof(PageHeader)) / sizeof(CellPointer))

enum PageType
{
	ROOT,
	INTERNAL,
	LEAF
};

struct DatabaseHeader
{
	char header_string[19];
	uint16_t page_size;
	uint32_t page_count;
};

struct PageHeader
{
	uint32_t id;
	PageType type;
	uint16_t free_start;
	uint16_t free_end;
	uint16_t total_free;
	uint16_t num_cells;
	uint32_t rightmost_pointer;
	uint8_t flags;
};

struct CellPointer
{
	uint16_t cell_location;
	uint16_t cell_size;
};

struct Cell
{
	uint32_t key;
	union
	{
		uint32_t child;
		char value[255];
	};
};

void addMasterPage(char *filename);
void *createPage(PageType type, uint32_t id);
void *allocatePage();
#endif
