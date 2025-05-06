#include "../../include/verify.h"

static VerificationResult verfiyCreate(CreateStmt *statement)
{
	puts(statement->table_name);
	return VERIFICATION_FAILED;
}

VerificationResult verifyStatement(Statement *statement)
{

	switch (statement->type)
	{
	case CREATE:
		return verfiyCreate(statement->create);

		break;

	default:
		return VERIFICATION_FAILED;
	}
}
