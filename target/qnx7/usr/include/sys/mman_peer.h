/*
 * $QNXLicenseC:
 * Copyright 2019 QNX Software Systems. All Rights Reserved.
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

/**
 * @file    mman_peer.h
 *
 * @brief   Extensions to memory-related functions that operate on a different
 *          (peer) process from the calling one
 */

#ifndef __MMAN_PEER_H_INCLUDED
#define __MMAN_PEER_H_INCLUDED

#ifndef __PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif

#ifndef __MMAN_H_INCLUDED
#include <sys/mman.h>
#endif

__BEGIN_DECLS

#if defined(__EXT_QNX)
extern void *mmap_peer(pid_t __pid, void *__addr, size_t __len, int __prot,
                       int __flags, int __fd, off_t __off);
extern int munmap_peer(pid_t __pid, void *__addr, size_t __len);

# if defined (__EXT_LF64SRC)
extern void *mmap64_peer(pid_t __pid, void *__addr, size_t __len, int __prot,
                         int __flags, int __fd, off64_t __off) __ALIASOFF("mmap_peer");
# endif
#endif

__END_DECLS

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/c/public/sys/mman_peer.h $ $Rev: 879464 $")
#endif
