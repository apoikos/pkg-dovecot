/* Copyright (C) 2002-2003 Timo Sirainen */

#include "common.h"

#ifdef PASSDB_SHADOW

#include "safe-memset.h"
#include "passdb.h"
#include "mycrypt.h"

#include <shadow.h>

#define SHADOW_CACHE_KEY "%u"
#define SHADOW_PASS_SCHEME "CRYPT"

static void
shadow_verify_plain(struct auth_request *request, const char *password,
		    verify_plain_callback_t *callback)
{
	struct spwd *spw;
	bool result;

	auth_request_log_debug(request, "shadow", "lookup");

	spw = getspnam(request->user);
	if (spw == NULL) {
		auth_request_log_info(request, "shadow", "unknown user");
		callback(PASSDB_RESULT_USER_UNKNOWN, request);
		return;
	}

	if (!IS_VALID_PASSWD(spw->sp_pwdp)) {
		auth_request_log_info(request, "shadow",
				      "invalid password field");
		callback(PASSDB_RESULT_USER_DISABLED, request);
		return;
	}

	/* save the password so cache can use it */
	auth_request_set_field(request, "password", spw->sp_pwdp,
			       SHADOW_PASS_SCHEME);

	/* check if the password is valid */
	result = strcmp(mycrypt(password, spw->sp_pwdp), spw->sp_pwdp) == 0;

	/* clear the passwords from memory */
	safe_memset(spw->sp_pwdp, 0, strlen(spw->sp_pwdp));

	if (!result) {
		auth_request_log_info(request, "shadow", "password mismatch");
		callback(PASSDB_RESULT_PASSWORD_MISMATCH, request);
		return;
	}

	/* make sure we're using the username exactly as it's in the database */
        auth_request_set_field(request, "user", spw->sp_namp, NULL);

	callback(PASSDB_RESULT_OK, request);
}

static void shadow_init(struct passdb_module *module,
			const char *args __attr_unused__)
{
	module->cache_key = SHADOW_CACHE_KEY;
	module->default_pass_scheme = SHADOW_PASS_SCHEME;
}

static void shadow_deinit(struct passdb_module *module __attr_unused__)
{
        endspent();
}

struct passdb_module_interface passdb_shadow = {
	"shadow",

	NULL,
	shadow_init,
	shadow_deinit,

	shadow_verify_plain,
	NULL
};

#endif
