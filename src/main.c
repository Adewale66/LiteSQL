#include "../include/processor.h"

#define DEVELOPMENT // to enable logging

int main(int argc, char *argv[])
{
	const char *filename = (argc == 2) ? argv[1] : NULL;
	run(filename);
	return 0;
}
