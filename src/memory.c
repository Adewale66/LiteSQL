#include "memory.h"

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
		fprintf(stderr, "Memory reallocation failed!\n");
		return NULL;
	}
	return result;
}

void *allocate(int size)
{
	void *result = malloc(size);
	if (result == NULL)
	{
		fprintf(stderr, "Memory reallocation failed!\n");
		return NULL;
	}

	return result;
}
