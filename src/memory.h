#ifndef _MEMORY
#define _MEMORY

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

#define ALLOCATE_MEMORY_WITH_ARGS(type, size, cleanup, args) \
	({                                                       \
		void *ptr = allocate(size);                          \
		if (!ptr)                                            \
		{                                                    \
			if (args != NULL)                                \
				cleanup(args);                               \
			exit(EXIT_FAILURE);                              \
		}                                                    \
		(type *)ptr;                                         \
	})

#define COPY_STRING(string, cleanup, arg)                   \
	({                                                      \
		char *ptr = strdup(string);                         \
		if (!ptr)                                           \
		{                                                   \
			fprintf(stderr, "Memory allocation failed!\n"); \
			cleanup(arg);                                   \
			exit(EXIT_FAILURE);                             \
		}                                                   \
		ptr;                                                \
	})

#define FREE_ARRAY(pointer) \
	free(pointer);

void *reallocate(void *pointer, size_t newSize);
void *allocate(int size);
#endif
