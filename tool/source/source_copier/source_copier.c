#include "source_copier.h"
#include <string.h>
#include <windows.h>
#include <shlwapi.h>
#ifdef __MINGW32__
	#include <shlobj.h>
#else
	#include <shlobj_core.h>
#endif
#include <make_string.h>
#include <hard_assert.h>

static char * compute_copy_path(enum argument directory);
static void ensure_destination_directory_exists(char destination_path[]);
static bool exists(char path[]);
static bool is_directory(char path[]);

void copy_source(enum argument origin_directory, enum argument destination_directory, bool allow_overwrites, bool delete_original)
{	char * origin = compute_copy_path(origin_directory),
	     * destination = compute_copy_path(destination_directory);

	ha_assert(exists(origin), MODULE_NAME, make_string("Missing copy origin file '%s'.", origin));

	if(!allow_overwrites && exists(destination))
		return;

	ensure_destination_directory_exists(destination);

	ha_assert
	(	CopyFileA(origin, destination, false),
		MODULE_NAME,
		make_string("CopyFileA failed with origin '%s', destination '%s' and error '%i'.", origin, destination, GetLastError())
	);

	if(delete_original)
		ha_assert(DeleteFileA(origin), MODULE_NAME, "DeleteFileA failure.");

	free(origin);
	free(destination);
}


static char * compute_copy_path(enum argument directory)
{	return strdup(make_string("\\\\?\\%s\\%s", arguments[directory], arguments[ARGUMENT_SOURCE_PATH]));
}

static void ensure_destination_directory_exists(char destination_path[])
{	char * directory = strdup(destination_path);
	ha_assert(PathRemoveFileSpecA(directory), MODULE_NAME, "PathRemoveFileSpecA failure.");

	if(!exists(directory))
		ha_assert
		(	!SHCreateDirectoryExA(NULL, directory, NULL),
			MODULE_NAME,
			make_string("Destination directory '%s' creation failure with error '%i'.", directory, GetLastError())
		);
	else
		ha_assert(is_directory(directory), MODULE_NAME, make_string("Destination directory path '%s' points to a file.", directory));

	free(directory);
}

static bool exists(char path[])
{	return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
}
static bool is_directory(char path[])
{	return exists(path) && (GetFileAttributesA(path) & FILE_ATTRIBUTE_DIRECTORY);
}
