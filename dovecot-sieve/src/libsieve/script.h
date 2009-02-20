/* script.h -- script definition
 * Larry Greenfield
 * $Id$
 */
/***********************************************************
        Copyright 1999 by Carnegie Mellon University

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Carnegie Mellon
University not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.

CARNEGIE MELLON UNIVERSITY DISCLAIMS ALL WARRANTIES WITH REGARD TO
THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS, IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY BE LIABLE FOR
ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
******************************************************************/

#ifndef SIEVE_SCRIPT_H
#define SIEVE_SCRIPT_H

#include <sys/types.h>

#include "sieve_interface.h"
#include "interp.h"
#include "tree.h"

#define ADDRERR_SIZE 500

struct sieve_script {
    sieve_interp_t interp;

    /* was a "require" done for these? */
    struct sieve_support {
	int fileinto       : 1;
	int reject         : 1;
	int envelope       : 1;
	int body           : 1;
	int vacation       : 1;
	int imapflags      : 1;
	int notify         : 1;
	int regex          : 1;
	int subaddress     : 1;
	int relational     : 1;
	int i_ascii_numeric: 1;
	int include        : 1;
	int copy           : 1;
    } support;

    void *script_context;
    commandlist_t *cmds;

    int err;
};

typedef struct sieve_bytecode sieve_bytecode_t;

struct sieve_bytecode {
    ino_t inode;		/* used to prevent mmapping the same script */
    const char *data;
    unsigned long len;
    int fd;

    int is_executing;		/* used to prevent recursive INCLUDEs */

    sieve_bytecode_t *next;
};

struct sieve_execute {
    sieve_bytecode_t *bc_list;	/* list of loaded bytecode buffers */
    sieve_bytecode_t *bc_cur;	/* currently active bytecode buffer */
};

/* generated by the yacc script */
commandlist_t *sieve_parse(sieve_script_t *script, FILE *f);
int script_require(sieve_script_t *s, char *req);

#endif
