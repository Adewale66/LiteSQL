#ifndef _MEMORY
#define _MEMORY

#include <stdlib.h>
#include <stdio.h>
#include "error.h"

#define GROW_CAPACITY(capacity) \
	((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
	(type *)reallocate(pointer, sizeof(type) * (newCount))

#define ALLOCATE_MEMORY(type, size) \
	(type *)allocate(size);

void *reallocate(void *pointer, size_t newSize);
void *allocate(int size);
#endif
