#include "../../include/memory.h"

static void alloc_failed()
{

	fprintf(stderr, "Memory allocation failed!\n");
	exit(EXIT_FAILURE);
}

void *reallocate(void *pointer, size_t newSize)
{
	if (newSize == 0)
	{
		free(pointer);
		return NULL;
	}

	void *result = realloc(pointer, newSize);
	if (!result)
	{
		alloc_failed();
	}
	return result;
}

void *allocate(int size)
{
	void *result = malloc(size);
	if (result == NULL)
	{
		alloc_failed();
	}

	return result;
}

char *copy(char *string)
{
	char *ptr = strdup(string);
	if (ptr == NULL)
	{
		alloc_failed();
	}
	return ptr;
}

void *allocate_page(int size)
{
	void *page = calloc(1, size);
	if (page == NULL)
	{
		alloc_failed();
	}
	return page;
}
