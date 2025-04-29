#include "../include/processor.h"

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

static void printStatement(Statement statement)
{
	switch (statement.type)
	{
	case CREATE:
		printCreate(statement.create);
		break;
	case SELECT:
		printSelect(statement.select);
		break;
	case DELETE:
		printDelete(statement.delete);
		break;
	case NULL_STMT:
	default:
		printf("NOT HANDLED YET\n");
	}
}

static void freeStatement(Statement statement)
{
	switch (statement.type)
	{
	case CREATE:
		freeCreate(statement.create);
		break;
	case SELECT:
		freeSelect(statement.select);
		break;
	case DELETE:
		freeDelete(statement.delete);
		break;
	case NULL_STMT:
	default:
		return;
	}
}

int run()
{

	InputBuffer *input = createBuffer();
	Scanner scanner;
	Statement statement;

	printf("LiteSQL version 1.0\nEnter \".help\" for usage hints\n");
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
			printStatement(statement);
			freeStatement(statement);
			freeInput(input);
		}
	}
	freeBuffer(input);
	return 0;
}
