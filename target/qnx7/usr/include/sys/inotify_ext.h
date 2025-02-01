/*
 * $QNXLicenseC: $
 */


#ifndef _SYS_INOTIFY_EXT_H_
#define _SYS_INOTIFY_EXT_H_

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdint.h>

__BEGIN_DECLS


#define INOTIFY_ABILITY_QNX_EXT		"fsevmgr/qnxext"
#define INOTIFY_ABILITY_RECURSE		"fsevmgr/recurse"


extern int inotify_qnx_ext(int fd, uint32_t extensions);


/*  QNX extensions to the inotify interface.  These flags are passed to the
	handle returned from inotify_init() using devctl(DCMD_FSEVMGR_QNX_EXT,
	INOTIFY_QNX_EXT_xxx) to enable one or more QNX extensions.
*/
#define INOTIFY_QNX_EXT_FREESPACE		0x00000001U	/* Free space notification */
#define INOTIFY_QNX_EXT_MOUNT			0x00000002U	/* Mount notification */
#define INOTIFY_QNX_EXT_UNMOUNT			0x00000004U	/* Unmount notification */
#define INOTIFY_QNX_EXT_RECURSE			0x00000008U	/* Allow recursive inotify */
#define INOTIFY_QNX_EXT_BIND_ADD		0x80000000U	/* Private - Notification for add binding */
#define INOTIFY_QNX_EXT_BIND_UPDATE		0x40000000U	/* Private - Notification for update binding */
#define INOTIFY_QNX_EXT_SECURITY		0x00000010U	/* Watch for security violations */
#define INOTIFY_QNX_EXT_MASK			0x0000001FU /* Set of extensions recognized by the implementation */


/*  Header used for all QNX extensions to the inotify interface
*/
typedef struct _inotify_qnx_ext_hdr {
	uint32_t	type;					/* One from the INOTIFY_QNX_EXT_xxx set */
	uint32_t	reserved;				/* Reserved - Set to zero */
} inotify_qnx_ext_hdr;


/*  Event format for INOTIFY_QNX_EXT_FREESPACE
*/
typedef struct {
	inotify_qnx_ext_hdr		hdr;		/* Header for the inotify QNX extensions */
	uint64_t				freespace;	/* Estimate of free space in bytes */
	uint32_t				namelen;	/* Length of the name */
	char 					name[0];	/* Zero terminated mount point */
} inotify_qnx_ext_freespace;


/*  Event format for INOTIFY_QNX_EXT_MOUNT and INOTIFY_QNX_EXT_UNMOUNT
*/
typedef struct {
	inotify_qnx_ext_hdr		hdr;		/* Header for the inotify QNX extensions */
	uint32_t				namelen;	/* Length of the name */
	char 					name[0];	/* Zero terminated mount point */
} inotify_qnx_ext_mount;


/*  Event format for INOTIFY_QNX_EXT_BIND_ADD and INOTIFY_QNX_EXT_BIND_UPDATE
*/
typedef struct {
	inotify_qnx_ext_hdr hdr;			/* Header for the inotify QNX extensions */
	int32_t parent_wd;					/* Watch descriptor for the parent */
	int32_t child_wd;					/* Watch descriptor for the child */
	char name[0];						/* Name associated with the child */
} inotify_qnx_ext_bind;


/*  Event format for INOTIFY_QNX_EXT_SECURITY
*/

enum inotify_qnx_ext_security_e {
	INOTIFY_QNX_EXT_SECURITY_BLOCK_VALIDATION_FAILED,
	INOTIFY_QNX_EXT_SECURITY_NUM
};

struct blk_validation_failure {
	uint64_t blkno;				/* physical block number */
	uint32_t hashblkno;			/* block within hash tree */
	uint16_t hashlevel;			/* level within hash tree */
	uint16_t hashblockoffset;	/* Offset within hash block */
};

typedef union fsnotify_sec_payload_u{
	struct blk_validation_failure blk_hash;
	uint8_t unused [32];				/* placeholder to set the minimum struct size */
} fsnotify_sec_payload_t;


typedef struct inotify_qnx_ext_security_s {
	inotify_qnx_ext_hdr hdr;			/* Header for the inotify QNX extensions */
	uint64_t inode;						/* Inode number triggering the violation */
	uint16_t subtype;					/* Event class subtype ENUM_INOTIFY_SECTYPE */
	uint16_t namelen;					/* length in bytes (including NUL) of name */
	uint32_t zero;						/* Unused */
	fsnotify_sec_payload_t payload;		/* Security event payload */
	char name[0];						/* If available, pathname impacted */
} inotify_qnx_ext_security_t;



__END_DECLS

#endif /* #ifndef _SYS_INOTIFY_EXT_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/BC700_6762_sdp704/lib/c/public/sys/inotify_ext.h $ $Rev: 864528 $")
#endif
