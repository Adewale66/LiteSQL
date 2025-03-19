#include "chunk.h"

void initChunk(Chunk *chunk)
{
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
}

void writeChunk(Chunk *chunk, uint8_t byte)
{
	if (chunk->capacity < chunk->count + 1)
	{
		int oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(oldCapacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, chunk->capacity);
	}

	chunk->code[chunk->count] = byte;
	chunk->count++;
}

void freeChunk(Chunk *chunk)
{
	FREE_ARRAY(chunk->code);
	initChunk(chunk);
}

Chunk *generateByteCode(ASTNode *node)
{
	Chunk *chunk = ALLOCATE_MEMORY_WITH_ARGS(Chunk, sizeof(Chunk), freeNode, node);
	initChunk(chunk);

	freeNode(node);
	return chunk;
}
