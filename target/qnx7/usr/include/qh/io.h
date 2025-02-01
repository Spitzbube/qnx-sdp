/*
 * $QNXLicenseC:
 * Copyright 2019, QNX Software Systems.  All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software.  Free development
 * licenses are available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#ifndef _QH_IO_H_INCLUDED
#define _QH_IO_H_INCLUDED

/**
 * @file io.h
 * QNX Helpers - IO related helpers.
 */

/* sorted headers from this project */
#ifndef _QH_QH_H_INCLUDED
#include "qh/qh.h"
#endif

/* sorted library headers */

/* sorted system headers */

/** @cond */
__BEGIN_DECLS
/** @endcond */

/**
 * Close a file and retries on EINTR.
 *
 * Closes a file associated with the specified file descriptor, and will retry when the close
 * function returns an error with errno set to EINTR.
 *
 * When the close() function is called, it can return an error with errno set to EINTR, in which
 * case the caller is responsible for calling it again, or else the fd will not be closed (fd leak).
 * qh_close() handles this for the caller, and also handles cases where the fd is -1, so the caller
 * doesn't need to wrap it (as is often done).
 *
 * @param[in] fd The file descriptor of the file you want to close (just like the one that would
 *               otherwise be passed to the close() function), or -1 (in which case the function
 *               does not call close() and returns successfully).
 *
 * @note This function will never return EINTR.
 *
 * @return EOK on success or a standard errno on failure (errno is then also set).
 */
static inline int
qh_close(int __fd)
{
    int retval__;

    if (__fd == -1) {
        return EOK;
    }

    do {
        retval__ = close(__fd);
    } while ((retval__ == -1) && (errno == EINTR));

    if (retval__ != -1) {
        return EOK;
    }

    return errno;
}

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QH_IO_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qh/public/qh/io.h $ $Rev: 897545 $")
#endif
