/* Copyright (c) 2006-2008 Dovecot Sieve authors, see the included COPYING file */

#include "common.h"
#include "str.h"

#include "commands.h"

#include "sieve-storage.h"
#include "sieve-storage-script.h"

#include <stdlib.h>

bool cmd_renamescript(struct client_command_context *cmd)
{
	struct client *client = cmd->client;
    struct sieve_storage *storage = client->storage;
    const char *scriptname, *newname;
    struct sieve_script *script;
    bool exists;

    /* <oldname> <newname> */
	if (!client_read_string_args(cmd, 2, &scriptname, &newname))
        return FALSE;

	exists = TRUE;
    script = sieve_storage_script_init(storage, scriptname, &exists);

    if (script == NULL) {
        if (!exists)
            client_send_no(client, "Script does not exist.");
        else
            client_send_storage_error(client, storage);

        return TRUE;
    }

    if (sieve_storage_script_rename(script, newname) < 0)
        client_send_storage_error(client, storage);
	else    
        client_send_ok(client, "Renamescript completed.");

	return TRUE;
}

