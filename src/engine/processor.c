#include "../../include/processor.h"

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

int run()
{
	// FILE *file;

	// file = fopen(argv[1], "rb");
	// if (file == NULL)
	// {
	// 	addMasterPage(argv[1]);
	// }

	// void *page = allocatePage();
	// size_t bytes_read = fread(page, PAGE_SIZE, 1, file);
	// if (bytes_read < 1)
	// {
	// 	fprintf(stderr, "Could not find page");
	// 	exit(EXIT_FAILURE);
	// }
	// PageHeader *pageHeader = (PageHeader *)((uint8_t *)page + sizeof(DatabaseHeader));
	// fclose(file);
	// free(page);

	InputBuffer *input = createBuffer();
	Scanner scanner;
	Statement statement;

	print_startup();

	while (true)
	{
		read_input(input);
		if (input->buffer[0] == '.')
		{
			if (strcmp(input->buffer, ".exit") == 0)
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
		}
		else
		{
			initScanner(&scanner, input->buffer, input->input_length);
			initParser(&statement, &scanner);
			VerificationResult result = verifyStatement(&statement);
			if (result == VERIFICATION_SUCCESS)
			{
				// generate bytcode
			}
			// printStatement(statement);
			freeStatement(statement);
			freeInput(input);
		}
	}
	freeBuffer(input);
	return 0;
}
