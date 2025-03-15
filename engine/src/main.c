#include "lexer.h"
#include "parser.h"
#include "stmt.h"

int main()
{
	char *statement = "SELECT idname, FROM unidentiferedtbalename;";
	initScanner(statement);
	TokenList tokens = scanTokens();
	if (tokens.size != 0)
	{

		Statement st = intiParser(tokens);
		if (st.statemntType == TOKEN_SELECT)
		{
			SelectStmt *stmt = st.stmt.select;
			printf("Column names: ");
			for (int i = 0; i < stmt->no_of_columns; i++)
			{
				printf("%s, ", stmt->column_names[i]);
			}
			printf("\nTable name: %s\n", stmt->table_name);
			printf("\nall tables: %d\n", stmt->all_tables);
		}

		// printTokens();
		freeStatment();
	}
}
