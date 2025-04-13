#pragma once

enum argument
{	ARGUMENT_BACKUP_DIRECTORY = 1,
	ARGUMENT_SOURCE_DIRECTORY,
	ARGUMENT_GUID,
	ARGUMENT_MODE,
	ARGUMENT_SOURCE_PATH,
	ARGUMENT_MAX
};

enum mode
{	MODE_BACKUP,
	MODE_PROCESS,
	MODE_RESTORE,
	MODE_MAX
};

extern char ** arguments;
extern enum mode mode;
