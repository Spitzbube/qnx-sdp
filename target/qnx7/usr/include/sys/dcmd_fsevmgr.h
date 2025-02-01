/*
 * $QNXLicenseC: $
 */


#ifndef __DCMD_FSEVMGR_H_INCLUDED
#define __DCMD_FSEVMGR_H_INCLUDED


#define DCMD_FSEVMGR_CHECK     		__DION(_DCMD_FSEVMGR, 1)
#define DCMD_FSEVMGR_FILTER_ADD		__DIOTF(_DCMD_FSEVMGR, 3, fsevmgr_watch_add_t)
#define DCMD_FSEVMGR_FILTER_REM		__DIOT(_DCMD_FSEVMGR, 4, int32_t)
#define DCMD_FSEVMGR_STATS			__DIOTF(_DCMD_FSEVMGR, 5, fsevmgr_stats_t)
#define DCMD_FSEVMGR_WRITER			__DIOTF(_DCMD_FSEVMGR, 6, fsevmgr_chidkey_t)
#define DCMD_FSEVMGR_AUTHORIZE		__DIOT(_DCMD_FSEVMGR, 7, uint64_t)
#define DCMD_FSEVMGR_QNX_EXT		__DIOT(_DCMD_FSEVMGR, 8, uint32_t)
#define DCMD_FSEVMGR_RFILTER_ADD	__DIOTF(_DCMD_FSEVMGR, 9, fsevmgr_watch_add_t)
#define DCMD_FSEVMGR_FSEVENTCHID	__DIOF(_DCMD_FSEVMGR, 10, fsevmgr_eventchid_t)
#define DCMD_FSEVMGR_INOTIFYCHID	__DIOF(_DCMD_FSEVMGR, 11, fsevmgr_eventchid_t)
#define DCMD_FSEVMGR_DBGLEVEL		__DIOT(_DCMD_FSEVMGR, 12, uint32_t)
#define DCMD_FSEVMGR_STATE			__DIOTF(_DCMD_FSEVMGR, 13, fsevmgr_state_t)
#define DCMD_FSEVMGR_MB_STATE		__DIOTF(_DCMD_FSEVMGR, 14, fsevmgr_mb_state_t)
#define DCMD_FSEVMGR_MB_RESTORE		__DIOT(_DCMD_FSEVMGR, 15, fsnotify_restore_t)


typedef struct fsevmgr_eventchid_s {
	pid_t pid;			/* Process id */
	int32_t chid;		/* Communication channel id */
} fsevmgr_eventchid_t;


typedef struct fsevmgr_chidkey_s {
	int32_t chid;		/* Communication channel id */
	uint32_t wrid;		/* Writer unique id */
	uint64_t key;		/* Populated with a communication key */
} fsevmgr_chidkey_t;


typedef struct fsevmgr_watch_add_s {
	int32_t wd;			/* Populated with a watch descriptor on success */
	uint32_t mask;		/* Combination of IN_xxx flags from inotify.h */
	int fd;				/* file descriptor for the path to watch */
} fsevmgr_watch_add_t;


/*  Definition of statistics tracked per event mailbox
*/
typedef struct fsevmgr_mbstats_s {
	uint64_t rxbytes;			/* Received total bytes  */
	uint64_t txbytes;			/* Transmitted total bytes  */
	uint64_t rxevents;			/* Received events */
	uint64_t txevents;			/* Transmitted events */
	uint64_t writecalls;		/* Number of write calls */
	uint64_t readcalls;			/* Number of read calls */
	uint64_t lostevents;		/* Number of lost events */
	uint32_t size;				/* Mailbox size in bytes */
	uint32_t used;				/* Mailbox bytes used */
	uint32_t maxused;			/* Mailbox maximum bytes used */
	uint32_t deferred;			/* Current deferred read count */
	uint32_t maxdeferred;		/* Maximum deferred read count */
	uint32_t watches;			/* Current number of watch filters */
	uint32_t maxwatches;		/* Maximum number of watch filters */
	uint32_t inotifymask;		/* Current combination of all inotify masks */
	uint32_t maxinotifymask;	/* Combination of all watch masks ever used */
	pid_t owner;				/* Owning process */
} fsevmgr_mbstats_t;


typedef struct fsevmgr_stats_s {
	uint32_t mailboxes;				/* Number of mailboxes (elements in array) */
	fsevmgr_mbstats_t mbstats[1];	/* Array of mailbox stats */
} fsevmgr_stats_t;


/*  Information about a mailbox as used by fsevmgr_state_t
*/
typedef struct fsevmgr_vmb_s {
	uint32_t muid;					/* Mailbox unique id */
	pid_t pid;						/* Owner id */
} fsevmgr_vmb_t;


/*  devctl for event manager state
*/
typedef struct fsevmgr_state_s {
    uint32_t count;            		/* Reported number of mailboxes */
	uint32_t actualcount;			/* Actual number of mailboxes */
    uint32_t maxcount;            	/* Maximum number of mailboxes */
    pid_t pid;						/* Event manager pid */
	fsevmgr_vmb_t mailbox[0];		/* Mailboxes associated with the event manager */
} fsevmgr_state_t;


/*  Information about a watch as used by fsevmgr_mb_state_t
*/
typedef struct fsevmgr_vwatch_s {
	uint32_t wuid;					/* Watch unique id */
	uint32_t mpuid;					/* Mount point unique id for the inode */
	uint64_t inode;					/* Inode number being watched */
	pid_t pid;						/* Server process id */
} fsevmgr_vwatch_t;


/*  devctl for mailbox state
*/
typedef struct fsevmgr_mb_state_s {
    uint32_t count;            		/* Reported number of watches */
	uint32_t actualcount;			/* Actual number of watches */
    uint32_t maxcount;            	/* Maximum number of watches */
	uint32_t muid;					/* Mailbox unique id */
	pid_t pid;						/* Owner id */
	fsevmgr_vwatch_t watch[0];		/* Watches associated with the mailbox */
} fsevmgr_mb_state_t;


/*  devctl for restoring a connection
*/
typedef struct fsnotify_restore_s {
	uint64_t key;					/* Writer key */
	uint32_t wrid;					/* Writer id */
	pid_t pid;						/* pid at time of save */
	int32_t newchid;				/* chid after restore */
} fsnotify_restore_t;


#endif  /* #ifndef __DCMD_FSEVMGR_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/BC700_6762_sdp704/services/blk/io-blk/public/sys/dcmd_fsevmgr.h $ $Rev: 777643 $")
#endif
