#include "processor.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <make_string.h>
#include <hard_assert.h>
#include "logger/logger.h"
#include "globals/globals.h"

static FILE * file;
static char * file_path;
static char * file_buffer;
static int file_buffer_length;
static bool is_string, is_preprocessor_instruction;

static void compute_file_path(void);
static void cleanup_file_path(void);
static void read_file(void);
static void cleanup_file_buffer(void);
static void open_file_for_writing(void);
static void close_file(void);
static void process_file_buffer(void);

void process(void)
{	log("processing '%s'", arguments[ARGUMENT_SOURCE_PATH]);
	compute_file_path();
	read_file();
	open_file_for_writing();
	process_file_buffer();
	close_file();
	cleanup_file_buffer();
	cleanup_file_path();
}

static void compute_file_path(void)
{	file_path = strdup(make_string("\\\\?\\%s\\%s", arguments[ARGUMENT_SOURCE_DIRECTORY], arguments[ARGUMENT_SOURCE_PATH]));
}

static void cleanup_file_path(void)
{	free(file_path);
}

static void read_file(void)
{	file = fopen(file_path, "rb");
	ha_assert(file, MODULE_NAME, make_string("Failed to open file '%s' with read permissions.", file_path));
	ha_assert(!fseek(file, 0, SEEK_END), MODULE_NAME, "fseek failure.");
	file_buffer_length = ftell(file);
	ha_assert(file_buffer_length >= 0, MODULE_NAME, "ftell failure.");
	rewind(file);
	file_buffer = malloc(file_buffer_length + 1);
	ha_assert(file_buffer, MODULE_NAME, "Memory allocation failure.");
	ha_assert(fread(file_buffer, sizeof(char), file_buffer_length, file) == file_buffer_length, MODULE_NAME, "fread failure.");
	close_file();
}

static void cleanup_file_buffer(void)
{	free(file_buffer);
}

static void open_file_for_writing(void)
{	file = fopen(file_path, "wb");
	ha_assert(file, MODULE_NAME, make_string("Failed to open file '%s' with write permissions.", file_path));
}

static void close_file(void)
{	ha_assert(!fclose(file), MODULE_NAME, "fclose failure");
}

static void process_file_buffer(void)
{	for(int i = 0; i < file_buffer_length; ++i)
	{	char character = file_buffer[i];
		switch(character)
		{	case '"':
				is_string = !is_string;
				break;
			case '#':
				if(!is_string)
					is_preprocessor_instruction = true;
				break;
			case '\r':
			case '\n':
				is_preprocessor_instruction = false;
				break;
			case '@':
				if(!is_string && !is_preprocessor_instruction)
				{	ha_assert
					(	fputs(make_string("internal2_%s_", arguments[ARGUMENT_GUID]), file) >= 0,
						MODULE_NAME,
						"fputs failure."
					);
					continue;
				}
		}
		ha_assert(fputc(character, file) == character, MODULE_NAME, "fputc failure.");
	}
}
