#ifndef __VM_h
#define __VM_h

#include <stdint.h>
#include "chunk.h"

#define REGISTER_SIZE 50

typedef struct
{
	uint8_t *pc;
	uint32_t registerStack[REGISTER_SIZE];
} VM;

typedef enum
{
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR,
	INTERPRET_UNKOWN_OPCODE
} InterpretResult;

void initVM(uint8_t *pc);
void resetVM();

#endif
