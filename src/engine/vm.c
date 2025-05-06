#include "../../include/vm.h"

VM vm;

void init(uint8_t *bytecode)
{
	vm.bytecode = bytecode;
	vm.ip = 0;
	vm.next_reg = 0;
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
