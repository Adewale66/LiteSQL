#ifndef _MEMORY
#define _MEMORY

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define GROW_CAPACITY(capacity) \
	((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, newCount) \
	(type *)reallocate(pointer, sizeof(type) * (newCount))

#define ALLOCATE_MEMORY(type, size) \
	(type *)allocate(size)

#define COPY_STRING(string) \
	copy(string)

#define FREE_ARRAY(pointer) \
	free(pointer);

void *reallocate(void *pointer, size_t newSize);
void *allocate(int size);
char *copy(char *string);

#endif
