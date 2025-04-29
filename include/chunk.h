#ifndef __CHUNK_H
#define __CHUNK_H

#include <stdint.h>
#include <stdlib.h>
#include "parser.h"
#include "memory.h"

// opcodes

#define OP_HALT 0x00

typedef struct Chunk Chunk;

struct Chunk
{
	int count;
	int capacity;
	uint8_t *code;
};

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte);
// void disassembleInstructions(Chunk *chunk);

#endif
