#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <make_string.h>

void log(char format[], ...)
{	va_list arguments;
	va_start(arguments, format);
	printf(MODULE_NAME "Internal2 : %s\n", make_string_variadic(format, arguments));
	va_end(arguments);
}
