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

#ifndef _QH_ENDIAN_H_INCLUDED
#define _QH_ENDIAN_H_INCLUDED

/**
 * @file endian.h
 *
 * QNX Helpers - Endianness related helpers.
 */

/* sorted headers from this project */
#ifndef _QH_QH_H_INCLUDED
#include "qh/qh.h"
#endif

/* sorted library headers */

/* sorted system headers */
#ifdef __QNXNTO__
#ifndef _GULLIVER_H_INCLUDED
#include <gulliver.h>
#endif
#else
#include <lib/compat_endian.h>
#endif

#ifndef _STDINT_H_INCLUDED
#include <stdint.h>
#endif

/** @cond */
__BEGIN_DECLS
/** @endcond */

/**
 * The following macros are versions of the non _CONST macros defined in NTO, but that can be used
 * in all headers and static initialization, being processed by the pre-processor.  The non _CONST
 * functions cannot be used this way because in some architectures, the macros end up executing
 * code.
 */

#define ENDIAN_RET16_CONST(__x) (uint16_t)((((uint16_t)(__x) >> 8) & UINT32_C(0xff)) | \
                                           (((uint8_t)(__x) & UINT32_C(0xff)) << 8))

#define ENDIAN_RET32_CONST(__x) (uint32_t)((((uint32_t)(__x) >> 24) & UINT32_C(0xff)) | \
                                           (((uint32_t)(__x) >> 8) & UINT32_C(0xff00)) | \
                                           (((uint32_t)(__x) & UINT32_C(0xff00)) << 8) | \
                                           (((uint32_t)(__x) & UINT32_C(0xff)) << 24))

#define ENDIAN_RET64_CONST(__x) (uint64_t)((((uint64_t)(__x) >> 56) & UINT64_C(0xff)) | \
                                           (((uint64_t)(__x) >> 40) & UINT64_C(0xff00)) | \
                                           (((uint64_t)(__x) >> 24) & UINT64_C(0xff0000)) | \
                                           (((uint64_t)(__x) >>  8) & UINT64_C(0xff000000)) | \
                                           (((uint64_t)(__x) & UINT64_C(0xff000000)) <<  8) | \
                                           (((uint64_t)(__x) & UINT64_C(0xff0000)) << 24) | \
                                           (((uint64_t)(__x) & UINT64_C(0xff00)) << 40) | \
                                           (((uint64_t)(__x) & UINT64_C(0xff)) << 56))

#if defined(__LITTLEENDIAN__)
#define ENDIAN_BE16_CONST(__x) ((uint16_t)(ENDIAN_RET16_CONST(__x)))
#define ENDIAN_BE32_CONST(__x) ((uint32_t)(ENDIAN_RET32_CONST(__x)))
#define ENDIAN_BE64_CONST(__x) ((uint64_t)(ENDIAN_RET64_CONST(__x)))
#define ENDIAN_LE16_CONST(__x) ((uint16_t)(__x))
#define ENDIAN_LE32_CONST(__x) ((uint32_t)(__x))
#define ENDIAN_LE64_CONST(__x) ((uint64_t)(__x))
#else
#define ENDIAN_BE16_CONST(__x) ((uint16_t)(__x))
#define ENDIAN_BE32_CONST(__x) ((uint32_t)(__x))
#define ENDIAN_BE64_CONST(__x) ((uint64_t)(__x))
#define ENDIAN_LE16_CONST(__x) ((uint16_t)(ENDIAN_RET16_CONST(__x)))
#define ENDIAN_LE32_CONST(__x) ((uint32_t)(ENDIAN_RET32_CONST(__x)))
#define ENDIAN_LE64_CONST(__x) ((uint64_t)(ENDIAN_RET64_CONST(__x)))
#endif

/**
 * The following are convenience macros (mainly for more obvious naming and type correctness).  Non
 * _CONST functions cannot be used in header files or for static initialization (they can execute
 * code under some architectures), but the _CONST versions are safe for use in those conditions.
 */

#define ENDIAN_HTOBE16(__x) ((uint16_t)ENDIAN_BE16((uint16_t)__x))
#define ENDIAN_HTOBE32(__x) ((uint32_t)ENDIAN_BE32((uint32_t)__x))
#define ENDIAN_HTOBE64(__x) ((uint64_t)ENDIAN_BE64((uint64_t)__x))
#define ENDIAN_HTOBE16_CONST(__x) ((uint16_t)ENDIAN_BE16_CONST((uint16_t)__x))
#define ENDIAN_HTOBE32_CONST(__x) ((uint32_t)ENDIAN_BE32_CONST((uint32_t)__x))
#define ENDIAN_HTOBE64_CONST(__x) ((uint64_t)ENDIAN_BE64_CONST((uint64_t)__x))
#define ENDIAN_BE16TOH(__x_be) ((uint16_t)ENDIAN_BE16((uint16_t)__x_be))
#define ENDIAN_BE32TOH(__x_be) ((uint32_t)ENDIAN_BE32((uint32_t)__x_be))
#define ENDIAN_BE64TOH(__x_be) ((uint64_t)ENDIAN_BE64((uint64_t)__x_be))
#define ENDIAN_BE16TOH_CONST(__x_be) ((uint16_t)ENDIAN_BE16_CONST((uint16_t)__x_be))
#define ENDIAN_BE32TOH_CONST(__x_be) ((uint32_t)ENDIAN_BE32_CONST((uint32_t)__x_be))
#define ENDIAN_BE64TOH_CONST(__x_be) ((uint64_t)ENDIAN_BE64_CONST((uint64_t)__x_be))

#define ENDIAN_HTOLE16(__x) ((uint16_t)ENDIAN_LE16((uint16_t)__x))
#define ENDIAN_HTOLE32(__x) ((uint32_t)ENDIAN_LE32((uint32_t)__x))
#define ENDIAN_HTOLE64(__x) ((uint64_t)ENDIAN_LE64((uint64_t)__x))
#define ENDIAN_HTOLE16_CONST(__x) ((uint16_t)ENDIAN_LE16_CONST((uint16_t)__x))
#define ENDIAN_HTOLE32_CONST(__x) ((uint32_t)ENDIAN_LE32_CONST((uint32_t)__x))
#define ENDIAN_HTOLE64_CONST(__x) ((uint64_t)ENDIAN_LE64_CONST((uint64_t)__x))
#define ENDIAN_LE16TOH(__x_le) ((uint16_t)ENDIAN_LE16((uint16_t)__x_le))
#define ENDIAN_LE32TOH(__x_le) ((uint32_t)ENDIAN_LE32((uint32_t)__x_le))
#define ENDIAN_LE64TOH(__x_le) ((uint64_t)ENDIAN_LE64((uint64_t)__x_le))
#define ENDIAN_LE16TOH_CONST(__x_le) ((uint16_t)ENDIAN_LE16_CONST((uint16_t)__x_le))
#define ENDIAN_LE32TOH_CONST(__x_le) ((uint32_t)ENDIAN_LE32_CONST((uint32_t)__x_le))
#define ENDIAN_LE64TOH_CONST(__x_le) ((uint64_t)ENDIAN_LE64_CONST((uint64_t)__x_le))

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QH_ENDIAN_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qh/public/qh/endian.h $ $Rev: 897735 $")
#endif
