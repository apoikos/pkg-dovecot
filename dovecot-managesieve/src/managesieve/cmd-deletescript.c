#include "common.h"
#include "commands.h"

#include "sieve-storage.h"
#include "sieve-storage-script.h"

bool cmd_deletescript(struct client_command_context *cmd)
{
	struct client *client = cmd->client;
	struct sieve_storage *storage = client->storage;
	const char *scriptname;
	struct sieve_script *script;
	bool exists;

	/* <scrip name>*/
	if (!client_read_string_args(cmd, 1, &scriptname))
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

	if (sieve_storage_script_delete(&script) < 0)
		client_send_storage_error(client, storage);
	else
		client_send_ok(client, "Deletescript completed.");

	/* Script object is deleted no matter what in 
	 * sieve_script_delete()
	 */

	return TRUE;
}
