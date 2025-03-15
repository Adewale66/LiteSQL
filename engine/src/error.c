#include "error.h"

void error(const char *message)
{
	fprintf(stderr, "%s\n", message);
}
