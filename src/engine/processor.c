#include "../../include/processor.h"

void *schema = NULL;

static InputBuffer *createBuffer()
{
	InputBuffer *buffer = (InputBuffer *)malloc(sizeof(InputBuffer));

	if (buffer == NULL)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	buffer->buffer = NULL;
	buffer->buffer_length = 0;
	buffer->input_length = 0;

	return buffer;
}

static void freeBuffer(InputBuffer *input)
{
	if (input->buffer != NULL)
		free(input->buffer);
	free(input);
}

static void freeInput(InputBuffer *input)
{
	free(input->buffer);
	input->buffer = NULL;
	input->buffer_length = 0;
}

static void read_input(InputBuffer *input)
{
	printf("LiteSQL> ");
	ssize_t bytes_read = getline(&(input->buffer), &(input->buffer_length), stdin);

	if (bytes_read <= 0)
	{
		fprintf(stderr, "Error reading input\n");
		exit(EXIT_FAILURE);
	}
	input->input_length = bytes_read - 1;
	input->buffer[bytes_read - 1] = '\0';
}

static void print_help()
{
	printf("HELP\n");
}

static void print_schema()
{
	printf("SCHEMA\n");
}

static void print_startup()
{
	const char *startup_message = "LiteSQL version 1.0\nEnter \".help\" for usage hints\n";
	write(STDOUT_FILENO, startup_message, strlen(startup_message));
}

int run(const char *filename)
{
	print_startup();
	if (filename == NULL)
	{
		write(STDOUT_FILENO, "Connected to a temporary database.\nUse \".open FILENAME\" to reopen on a persistent database.\n", 93);
	}
	schema = initSchemaPage(filename);

	InputBuffer *input = createBuffer();

	while (true)
	{
		read_input(input);
		if (input->buffer[0] == '.')
		{
			if (strcmp(input->buffer, ".quit") == 0)
			{
				break;
			}
			if (strcmp(input->buffer, ".help") == 0)
			{
				print_help();
			}
			if (strcmp(input->buffer, ".schema") == 0)
			{
				print_schema();
			}
			if (strstr(input->buffer, ".open"))
			{
				const char *db_name = 6 + input->buffer; // slice off ".open " from the input
				if (schema != NULL)
				{
					free(schema);
					schema = NULL;
				}
				schema = initSchemaPage(db_name);
				write(STDOUT_FILENO, "Database updated.\n", 19);
			}
		}
		else
		{
			Scanner *scanner = initScanner(input->buffer, input->input_length);
			Statement *statement = prepare_statement(scanner);
			// VerificationResult result = verifyStatement(&statement, schema);
			// if (result == VERIFICATION_SUCCESS)
			// {
			// 	// generate bytcode
			// }
			print_statement(statement);
			free_statement(statement);
			freeInput(input);
		}
	}
	freeBuffer(input);
	free(schema);
	return 0;
}
