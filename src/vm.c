#include "../include/vm.h"

VM vm;

void init(uint8_t *pc)
{
	vm.pc = pc;
}

void resetVM()
{
	vm = (typeof(VM)){NULL};
}

// static InterpretResult createByteCode(CreateStmt *create)
// {
// }

InterpretResult generateBytecCode(Statement *statement)
{
	switch (statement->type)
	{
	case CREATE:
		// return createByteCode(statement->create);
		break;

	default:
		break;
	}
	return INTERPRET_OK;
}
