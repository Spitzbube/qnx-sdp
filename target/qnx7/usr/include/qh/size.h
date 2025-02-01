/*
 * $QNXLicenseC:
 * Copyright 2017-2019, QNX Software Systems.  All Rights Reserved.
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

#ifndef _QH_SIZE_H_INCLUDED
#define _QH_SIZE_H_INCLUDED

/**
 * @file size.h
 * QNX Helpers - Size related helpers.
 */

/* sorted headers from this project */
#ifndef _QH_QH_H_INCLUDED
#include "qh/qh.h"
#endif

/* sorted library headers */

/* sorted system headers */
#ifndef _LIMITS_H_INCLUDED
#include <limits.h>
#endif

#ifndef _STDDEF_H_INCLUDED
#include <stddef.h>
#endif

#ifndef _STDINT_H_INCLUDED
#include <stdint.h>
#endif

/** @cond */
__BEGIN_DECLS
/** @endcond */

/**
 * Provides the size of a member of a composite type (struct, union), provided it's type and the
 * member name.
 *
 * @param[in] composite The composite type.
 * @param[in] member    The name of the member who's size is to be determined.
 *
 * @return The size of the member of the composite type.
 */
#define qh_sizeof_member(__composite, __member) sizeof(((__composite *)0)->__member)

/**
 * Provides the size of a portion of a composite type (struct, union), up to and including a
 * provided member name.
 *
 * @param[in] composite  The composite type.
 * @param[in] inc_member The name of the last member to be included in the determination of the size
 *                       of the composite portion.
 *
 * @return The size of a portion of the composite type, up to and including the provided member
 *         name.
 */
#define qh_sizeof_utai_member(__composite, __inc_member) \
            (offsetof(__composite, __inc_member) + qh_sizeof_member(__composite, __inc_member))

/**
 * The QH_MAX_PRINTSIZE_* macros contain the maximum length of a number of the specified type when
 * printed (base 10).  With signed types, the negative sign is included in the length.
 */
#define QH_MAX_PRINTSIZE_INT8                                                            ((size_t)4)
#define QH_MAX_PRINTSIZE_INT16                                                           ((size_t)6)
#define QH_MAX_PRINTSIZE_INT32                                                          ((size_t)11)
#define QH_MAX_PRINTSIZE_INT64                                                          ((size_t)20)

#define QH_MAX_PRINTSIZE_UINT8                                                           ((size_t)3)
#define QH_MAX_PRINTSIZE_UINT16                                                          ((size_t)5)
#define QH_MAX_PRINTSIZE_UINT32                                                         ((size_t)10)
#define QH_MAX_PRINTSIZE_UINT64                                                         ((size_t)20)

#if INT_MAX == INT32_MAX
# define QH_MAX_PRINTSIZE_INT                                                 QH_MAX_PRINTSIZE_INT32
# define QH_MAX_PRINTSIZE_UINT                                               QH_MAX_PRINTSIZE_UINT32
#elif INT_MAX == INT64_MAX
# define QH_MAX_PRINTSIZE_INT                                                 QH_MAX_PRINTSIZE_INT64
# define QH_MAX_PRINTSIZE_UINT                                               QH_MAX_PRINTSIZE_UINT64
#else
# error Unable to determine 'int' print size
#endif

#if LONG_MAX == INT32_MAX
# define QH_MAX_PRINTSIZE_LONG                                                QH_MAX_PRINTSIZE_INT32
# define QH_MAX_PRINTSIZE_ULONG                                              QH_MAX_PRINTSIZE_UINT32
#elif LONG_MAX == INT64_MAX
# define QH_MAX_PRINTSIZE_LONG                                                QH_MAX_PRINTSIZE_INT64
# define QH_MAX_PRINTSIZE_ULONG                                              QH_MAX_PRINTSIZE_UINT64
#else
# error Unable to determine 'long' print size
#endif

#if LLONG_MAX == INT64_MAX
# define QH_MAX_PRINTSIZE_LLONG                                               QH_MAX_PRINTSIZE_INT64
# define QH_MAX_PRINTSIZE_ULLONG                                             QH_MAX_PRINTSIZE_UINT64
#else
# error Unable to determine 'long long' print size
#endif

#define QH_MAX_PRINTSIZE_SSIZE32                                              QH_MAX_PRINTSIZE_INT32
#define QH_MAX_PRINTSIZE_SIZE32                                              QH_MAX_PRINTSIZE_UINT32

#define QH_MAX_PRINTSIZE_SSIZE64                                              QH_MAX_PRINTSIZE_INT64
#define QH_MAX_PRINTSIZE_SIZE64                                              QH_MAX_PRINTSIZE_UINT64

#if SSIZE_MAX == INT32_MAX
# define QH_MAX_PRINTSIZE_SSIZE                                               QH_MAX_PRINTSIZE_INT32
# define QH_MAX_PRINTSIZE_SIZE                                               QH_MAX_PRINTSIZE_UINT32
#elif SSIZE_MAX == INT64_MAX
# define QH_MAX_PRINTSIZE_SSIZE                                               QH_MAX_PRINTSIZE_INT64
# define QH_MAX_PRINTSIZE_SIZE                                               QH_MAX_PRINTSIZE_UINT64
#else
# error Unable to determine 'ssize_t' print size
#endif

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QH_SIZE_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qh/public/qh/size.h $ $Rev: 897545 $")
#endif
