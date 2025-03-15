#include "memory.h"

static void *memoryAllocFailed()
{
	// THIS WOULD SEND TO THE CLIENT (TODO)
	fprintf(stderr, "Memory reallocation failed!");
	return NULL;
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
		return memoryAllocFailed();
	}
	return result;
}

void *allocate(int size)
{
	void *result = malloc(size);
	if (result == NULL)
	{
		return memoryAllocFailed();
	}

	return result;
}
