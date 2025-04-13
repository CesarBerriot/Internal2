#include "mode_manager.h"
#include <string.h>
#include <hard_assert.h>
#include <make_string.h>
#include "globals/globals.h"
#include "up_backer/up_backer.h"
#include "processor/processor.h"
#include "restorer/restorer.h"

void parse_mode(void)
{	struct { enum mode mode; char * name; } modes[MODE_MAX] =
	{	{ .mode = MODE_BACKUP, .name = "backup" },
		{ .mode = MODE_PROCESS, .name = "process" },
		{ .mode = MODE_RESTORE, .name = "restore" },
	};
	for(enum mode i = 0; i < MODE_MAX; ++i)
		if(!strcmp(modes[i].name, arguments[ARGUMENT_MODE]))
		{	mode = modes[i].mode;
			return;
		}
	ha_abort(MODULE_NAME, "Invalid mode argument.");
}

void run_selected_mode(void)
{	typedef void(*procedure_t)(void);

	procedure_t procedures[MODE_MAX] =
	{	[MODE_BACKUP] = backup,
		[MODE_PROCESS] = process,
		[MODE_RESTORE] = restore,
	};

	procedure_t procedure = procedures[mode];
	ha_assert(procedure, MODULE_NAME, make_string("Missing procedure for mode %i.", mode));
	procedure();
}
