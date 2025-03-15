#ifndef __PARSER_H
#define __PARSER_H

#include <stdarg.h>
#include "stmt.h"
#include "errno.h"

typedef struct
{
	TokenList tokenlist;
	int current;

} Parser;

Statement intiParser(TokenList tokens);
void freeStatment();
#endif
