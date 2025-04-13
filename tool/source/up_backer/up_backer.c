#include "up_backer.h"
#include "globals/globals.h"
#include "logger/logger.h"
#include "source_copier/source_copier.h"

void backup(void)
{	log("backing up '%s' to '%s'", arguments[ARGUMENT_SOURCE_PATH], arguments[ARGUMENT_BACKUP_DIRECTORY]);
	copy_source(ARGUMENT_SOURCE_DIRECTORY, ARGUMENT_BACKUP_DIRECTORY, false, false);
}
