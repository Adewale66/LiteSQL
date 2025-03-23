#ifndef __PROCESSOR_H
#define __PROCESSOR_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

typedef struct InputBuffer InputBuffer;

struct InputBuffer
{
	char *buffer;
	size_t buffer_length;
	ssize_t input_length;
};

int run();

#endif
