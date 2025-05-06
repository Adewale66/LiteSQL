#ifndef __VM_h
#define __VM_h

#include <stdint.h>

#include "chunk.h"

#define REGISTER_SIZE 200

typedef struct
{
	uint8_t *bytecode;
	uint8_t *ip;
	int next_reg;
	uint32_t registers[REGISTER_SIZE];
} VM;

typedef enum
{
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_UNKOWN_OPCODE
} InterpretResult;

void initVM(uint8_t *pc);
void resetVM();
InterpretResult generateBytecCode(Statement *statement);

#endif
