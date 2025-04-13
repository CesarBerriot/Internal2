#include <stdlib.h>
#include <make_string.h>
#include <hard_assert.h>
#include "mode_manager/mode_manager.h"
#include "globals/globals.h"

static void check_arguments_amount(int);
static void convert_arguments_to_backslashes(void);

int main(int arguments_amount, char ** arguments_argument)
{	check_arguments_amount(arguments_amount);
	arguments = arguments_argument;
	convert_arguments_to_backslashes();
	parse_mode();
	run_selected_mode();
	return EXIT_SUCCESS;
}

static void check_arguments_amount(int arguments_amount)
{	ha_assert(arguments_amount == ARGUMENT_MAX, MODULE_NAME, make_string("Invalid amount of arguments (%i).", arguments_amount));
}

static void convert_arguments_to_backslashes(void)
{	for(int i_argument = 0; i_argument < ARGUMENT_MAX; ++i_argument)
		for(char * i_character = arguments[i_argument]; *i_character; ++i_character)
			if(*i_character == '/')
				*i_character = '\\';
}
