#ifndef __SIEVE_LIST_H
#define __SIEVE_LIST_H

#include "lib.h"
#include "str.h"
#include "sieve-storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

struct sieve_list_context;

/* Create a context for listing the scripts in the storage */
struct sieve_list_context *sieve_storage_list_init
	(struct sieve_storage *storage);

/* Get the next script in the storage. */
const char *sieve_storage_list_next(struct sieve_list_context *ctx, bool *active);

/* Destroy the listing context */
int sieve_storage_list_deinit(struct sieve_list_context **ctx);

#endif


	
    
