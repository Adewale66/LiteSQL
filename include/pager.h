#ifndef __PAGES_H
#define __PAGES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "memory.h"

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
	LEAF = 0x0D,
	INTERNAL = 0x05
};

struct DatabaseHeader
{
	uint32_t id;
	char header_string[19];
	char filename[255];
	uint16_t page_size;
	uint32_t page_count;
	PageType type;
	uint16_t free_start;
	uint16_t free_end;
	uint16_t total_free;
	uint16_t num_rows;
	uint32_t rightmost_pointer;
	uint8_t flags;
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

void *createPage(PageType type, uint32_t id);
void *initSchemaPage(const char *filename);

#endif
