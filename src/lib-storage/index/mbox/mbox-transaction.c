/* Copyright (C) 2004 Timo Sirainen */

#include "lib.h"
#include "mbox-storage.h"
#include "mbox-lock.h"
#include "mbox-sync-private.h"

struct mailbox_transaction_context *
mbox_transaction_begin(struct mailbox *box,
		       enum mailbox_transaction_flags flags)
{
	struct mbox_mailbox *mbox = (struct mbox_mailbox *)box;
	struct mbox_transaction_context *t;

	t = i_new(struct mbox_transaction_context, 1);
	index_transaction_init(&t->ictx, &mbox->ibox, flags);
	return &t->ictx.mailbox_ctx;
}

int mbox_transaction_commit(struct mailbox_transaction_context *_t,
			    enum mailbox_sync_flags flags)
{
	struct mbox_transaction_context *t =
		(struct mbox_transaction_context *)_t;
	struct mbox_mailbox *mbox = (struct mbox_mailbox *)t->ictx.ibox;
	unsigned int lock_id = t->mbox_lock_id;
	bool mbox_modified;
	int ret = 0;

	if (t->save_ctx != NULL)
		ret = mbox_transaction_save_commit(t->save_ctx);
	mbox_modified = t->mbox_modified;

	if (ret == 0) {
		if (index_transaction_commit(_t) < 0)
			ret = -1;
	} else {
		index_transaction_rollback(_t);
	}
	t = NULL;

	if (lock_id != 0 && mbox->mbox_lock_type != F_WRLCK) {
		/* unlock before writing any changes */
		(void)mbox_unlock(mbox, lock_id);
		lock_id = 0;
	}

	if (ret == 0) {
		enum mbox_sync_flags mbox_sync_flags = MBOX_SYNC_LAST_COMMIT;
		if ((flags & MAILBOX_SYNC_FLAG_FULL_READ) != 0 &&
		    !mbox->mbox_very_dirty_syncs)
			mbox_sync_flags |= MBOX_SYNC_UNDIRTY;
		if ((flags & MAILBOX_SYNC_FLAG_FULL_WRITE) != 0)
			mbox_sync_flags |= MBOX_SYNC_REWRITE;
		if (mbox_modified) {
			/* after saving mails we want to update the last-uid */
			mbox_sync_flags |= MBOX_SYNC_HEADER | MBOX_SYNC_REWRITE;
		}
		if (mbox_sync(mbox, mbox_sync_flags) < 0)
			ret = -1;
	}

	if (lock_id != 0) {
		if (mbox_unlock(mbox, lock_id) < 0)
			ret = -1;
	}
	return ret;
}

void mbox_transaction_rollback(struct mailbox_transaction_context *_t)
{
	struct mbox_transaction_context *t =
		(struct mbox_transaction_context *)_t;
	struct mbox_mailbox *mbox = (struct mbox_mailbox *)t->ictx.ibox;

	if (t->save_ctx != NULL)
		mbox_transaction_save_rollback(t->save_ctx);

	if (t->mbox_lock_id != 0)
		(void)mbox_unlock(mbox, t->mbox_lock_id);
	index_transaction_rollback(_t);
}
