#ifndef __PROCESSOR_H
#define __PROCESSOR_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "debug.h"
#include "verify.h"

typedef struct InputBuffer InputBuffer;

struct InputBuffer
{
	char *buffer;
	size_t buffer_length;
	ssize_t input_length;
};

int run();

#endif
