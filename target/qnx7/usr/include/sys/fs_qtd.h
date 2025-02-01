/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software.   Free development
 * licenses are available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#ifndef __SYS_FS_QTD_H_INCLUDED
#define __SYS_FS_QTD_H_INCLUDED

#include <inttypes.h>
#include <unistd.h>

#include <sys/cdefs.h>

__BEGIN_DECLS

#define QTD_SUPERBLOCK_MAGIC	0xc5c6c7c8
#define QTD_SUPERBLOCK_VERSION	0x1
#define QTD_SUPERBLOCK_SIZE		4096
#define QTD_SALT_MAX_SIZE		128			/* 2x size of max digest size */
#define QTD_DIGEST_MAX_SIZE		64			/* maximum supported digest length */
#define QTD_LEVELS_MAX			10			/* Max number of merkle tree levels */
#define QTD_BLOCKSIZE_MIN		512			/* minimum block size */
#define QTD_BLOCKSIZE_MAX		32768		/* maximum block size */

/*
 * Signature Algorithms
 */
typedef enum {
	QTD_SIG_INVALID = 0,		/* trap invalid sig types */
	QTD_SIG_RSAWITHSHA256,		/* RSA with SHA 256 */
	QTD_SIG_ECDSAWITHSHA256,	/* ECDSA with SHA 256 */
	QTD_SIG_MAX,
} qtd_sig_t;

/*
 * Hash Algorithms
 */
typedef enum {
	QTD_HASH_INVALID = 0,		/* trap invalid hash values */
	QTD_HASH_SHA1,
	QTD_HASH_SHA256,
	QTD_HASH_SHA512,
	QTD_HASH_MAX,
} qtd_hash_t;

/*
 * Option Flags
 */
typedef enum {
	QTD_PARTITION_MODE = 0x01,	/* full partition mode */
} qtd_flag_t;

/*
 * QTD Superblock
 */
typedef union __attribute__((__packed__)) _qtd_superblock {
	struct {
		uint32_t				magic;								/* magic marker */
		uint32_t				version;							/* version */
		uint32_t				flags;								/* flags */
		uint32_t				blksize;							/* block size */
		uint32_t				hblksize;							/* hash block size */
		uint32_t				hashtype;							/* hash type */
		uint64_t				fssize;								/* total size of inner fs */
		uint64_t				totalsize;							/* whole image size covered by qtd */
		uint64_t				offset;								/* where the merkle meta data starts */
		uint32_t				sigtype;							/* sig type */
		uint32_t				saltsize;							/* salt size */
		uint8_t					salt[QTD_SALT_MAX_SIZE];			/* salt */
		uint64_t				rollback;							/* rollback version */
		uint32_t				pathtrust;							/* pathtrust trust field (unused) */
		uint8_t					roothash[QTD_DIGEST_MAX_SIZE];		/* root hash */
		uint32_t				sigsize;							/* signature size */
		uint8_t					sig[0];								/* signature */
	};
	char						filler[QTD_SUPERBLOCK_SIZE];
} qtd_superblock_t;

/* static size check to make sure that the superblock is of proper size */
typedef char __qtd_sb_size_check[(sizeof(qtd_superblock_t) == QTD_SUPERBLOCK_SIZE) ? 1 : -1];

__END_DECLS

#endif /* __SYS_FS_QTD_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/BC700_6762_sdp704/services/blk/fs/qtd/public/sys/fs_qtd.h $ $Rev: 859952 $")
#endif
