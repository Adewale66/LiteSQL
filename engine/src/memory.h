#ifndef _MEMORY
#define _MEMORY

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "error.h"

#define GROW_CAPACITY(capacity) \
	((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, newCount) \
	(type *)reallocate(pointer, sizeof(type) * (newCount))

#define ALLOCATE_MEMORY(type, size, cleanup) \
	({                                       \
		void *ptr = allocate(size);          \
		if (!ptr)                            \
		{                                    \
			cleanup();                       \
			exit(EXIT_FAILURE);              \
		}                                    \
		(type *)ptr;                         \
	})
#define COPY_STRING(string, cleanup)            \
	({                                          \
		char *ptr = strdup(string);             \
		if (!ptr)                               \
		{                                       \
			error("Memory allocation failed!"); \
			cleanup();                          \
			exit(EXIT_FAILURE);                 \
		}                                       \
		ptr;                                    \
	})
void *reallocate(void *pointer, size_t newSize);
void *allocate(int size);
#endif
