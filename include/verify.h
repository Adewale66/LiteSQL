#ifndef __VERIFY
#define __VERIFY

#include "parser.h"

typedef enum
{
	VERIFICATION_SUCCESS,
	VERIFICATION_FAILED,
} VerificationResult;

VerificationResult verifyStatement(Statement *statement, void *schema);

#endif
