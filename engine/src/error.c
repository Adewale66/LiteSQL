#include "error.h"

void error(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
}
