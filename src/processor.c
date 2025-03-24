#include "processor.h"

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
			// handle meta command
		}
		else
		{
			initScanner(&scanner, input->buffer, input->input_length);
			initParser(&statement, &scanner);
			printSelect(&(statement.select));
			freeSelect(&(statement.select));
		}
	}
	freeBuffer(input);
}
