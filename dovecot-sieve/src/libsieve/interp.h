/* interp.h -- interpretor definition
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
*****************************************************************/

#ifndef SIEVE_INTERP_H
#define SIEVE_INTERP_H

#include "sieve_interface.h"

enum enum_value {

  IMAP_ENUM_ZERO = 0,

  IMAP_ENUM_SIEVE_EXTENSIONS_COPY = (1<<11),
  IMAP_ENUM_SIEVE_EXTENSIONS_SUBADDRESS = (1<<10),
  IMAP_ENUM_SIEVE_EXTENSIONS_REGEX = (1<<9),
  IMAP_ENUM_SIEVE_EXTENSIONS_RELATIONAL = (1<<8),
  IMAP_ENUM_SIEVE_EXTENSIONS_BODY = (1<<7),
  IMAP_ENUM_SIEVE_EXTENSIONS_ENVELOPE = (1<<6),
  IMAP_ENUM_SIEVE_EXTENSIONS_INCLUDE = (1<<5),
  IMAP_ENUM_SIEVE_EXTENSIONS_NOTIFY = (1<<4),
  IMAP_ENUM_SIEVE_EXTENSIONS_IMAPFLAGS = (1<<3),
  IMAP_ENUM_SIEVE_EXTENSIONS_VACATION = (1<<2),
  IMAP_ENUM_SIEVE_EXTENSIONS_REJECT = (1<<1),
  IMAP_ENUM_SIEVE_EXTENSIONS_FILEINTO = (1<<0)
};
#define EXTENSIONS_ALL ((1 << 12)-1)

struct sieve_interp {
    /* standard callbacks for actions */
    sieve_callback *redirect, *discard, *reject, *fileinto, *keep;
    sieve_callback *notify;
    sieve_vacation_t *vacation;

    sieve_get_size *getsize;
    sieve_get_header *getheader;
    sieve_get_envelope *getenvelope;
    sieve_get_body *getbody;
    sieve_get_include *getinclude;

    sieve_parse_error *err;

    /* site-specific imapflags for mark/unmark */
    sieve_imapflags_t *markflags;

    sieve_execute_error *execute_err;

    /* context to pass along */
    void *interp_context;
};

int interp_verify(sieve_interp_t *interp);

#endif
