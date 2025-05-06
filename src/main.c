#include "../include/processor.h"

#define DEVELOPMENT // to enable logging

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: ./bin/db <filename>\n");
		fprintf(stderr, "Usage: ./bin/db <filename>%s", argv[1]);
		exit(EXIT_FAILURE);
	}

	run();

	return 0;
}
