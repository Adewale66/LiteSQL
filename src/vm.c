#include "vm.h"

VM vm;

void init(uint8_t *pc)
{
	vm.pc = pc;
}

void resetVM()
{
	vm = (typeof(VM)){NULL};
}
