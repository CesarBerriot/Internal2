#include "restorer.h"
#include "logger/logger.h"
#include "source_copier/source_copier.h"

void restore(void)
{	log("restoring '%s'", arguments[ARGUMENT_SOURCE_PATH]);
	copy_source(ARGUMENT_BACKUP_DIRECTORY, ARGUMENT_SOURCE_DIRECTORY, true, true);
}
