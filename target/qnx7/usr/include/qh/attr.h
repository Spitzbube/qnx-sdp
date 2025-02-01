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

#ifndef _QH_ATTR_H_INCLUDED
#define _QH_ATTR_H_INCLUDED

/**
 * @file attr.h
 *
 * QNX Helpers - QH specific attribute helpers.
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

#if defined (__GNUC__)

#if defined(__MINGW32__) || defined(__MINGW64__)
/*
 * Note that in order for this to work properly __USE_MINGW_ANSI_STDIO should be defined prior to
 * including any QH header file as they include stdio.h.  Failure to do so would result in MS's
 * version of printf to be used instead of MinGW's version, and it does not support C99 format
 * attributes.  The safer way to do so is to define at the project level when building for MingGW
 * (e.g. CPPFLAGS += -D__USE_MINGW_ANSI_STDIO=1)
 */
#define QH_ATTR_FUNC_FORMAT_PRINTF(__STRING_INDEX, __FIRST_TO_CHECK)  \
        __attribute__((__format__(__MINGW_PRINTF_FORMAT, (__STRING_INDEX), (__FIRST_TO_CHECK))))
#else
#define QH_ATTR_FUNC_FORMAT_PRINTF(__STRING_INDEX, __FIRST_TO_CHECK)  \
        __attribute__((__format__(__printf__, (__STRING_INDEX), (__FIRST_TO_CHECK))))
#endif

#if defined(__DARWIN__)
/* On Darwin, priorities are not supported. */
#define QH_ATTR_FUNC_CONSTRUCTOR_PRIORITY(__PRIORITY)  \
        __attribute__((__constructor__))
#define QH_ATTR_FUNC_DESTRUCTOR_PRIORITY(__PRIORITY)  \
        __attribute__((__destructor__))
#else
#define QH_ATTR_FUNC_CONSTRUCTOR_PRIORITY(__PRIORITY)  \
        __attribute__((__constructor__(__PRIORITY)))
#define QH_ATTR_FUNC_DESTRUCTOR_PRIORITY(__PRIORITY)  \
        __attribute__((__destructor__(__PRIORITY)))
#endif

#define QH_ATTR_FUNC_NONNULL_PARAMS(...) __attribute__((__nonnull__(__VA_ARGS__)))

#define QH_ATTR_FUNC_DEPRECATED(__MSG) __attribute__((__deprecated__((__MSG))))

#if (__GNUC__ + 0) >= 6
#define QH_ATTR_ENUM_DEPRECATED(__MSG) __attribute__((__deprecated__((__MSG))))
#else
#define QH_ATTR_ENUM_DEPRECATED(__MSG)
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define QH_ATTR_FUNC_VISIBILITY(__VISIBILITY)
#else
#define QH_ATTR_FUNC_VISIBILITY(__VISIBILITY) __attribute__((__visibility__((__VISIBILITY))))
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define QH_ATTR_VAR_VISIBILITY(__VISIBILITY)
#else
#define QH_ATTR_VAR_VISIBILITY(__VISIBILITY) __attribute__((__visibility__((__VISIBILITY))))
#endif

#else /* defined(__GNUC__) */

#define QH_ATTR_FUNC_FORMAT_PRINTF(__STRING_INDEX, __FIRST_TO_CHECK)
#define QH_ATTR_FUNC_CONSTRUCTOR_PRIORITY(__PRIORITY)
#define QH_ATTR_FUNC_DESTRUCTOR_PRIORITY(__PRIORITY)
#define QH_ATTR_FUNC_NONNULL_PARAMS(...)
#define QH_ATTR_FUNC_VISIBILITY(__VISIBILITY)
#define QH_ATTR_FUNC_DEPRECATED(__MSG)
#define QH_ATTR_ENUM_DEPRECATED(__MSG)
#define QH_ATTR_VAR_VISIBILITY(__VISIBILITY)

#endif

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QH_ATTR_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qh/public/qh/attr.h $ $Rev: 901920 $")
#endif
