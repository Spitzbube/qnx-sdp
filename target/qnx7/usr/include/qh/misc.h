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

#ifndef _QH_MISC_H_INCLUDED
#define _QH_MISC_H_INCLUDED

/**
 * @file misc.h
 * QNX Helpers - miscellaneous helpers.
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
 * Convenience macro marking a variable as unused to prevent compiler warnings.
 */
#define QH_UNUSED(__unused) ((void)(__unused))

/**
 * Convenience macro determining the length (number of elements) of an array.
 */
#define QH_ARRAY_LENGTH(__arr) (sizeof(__arr) / sizeof((__arr)[0]))

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QH_MISC_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qh/public/qh/misc.h $ $Rev: 901949 $")
#endif
