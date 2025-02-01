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

#ifndef _QH_QH_H_INCLUDED
#define _QH_QH_H_INCLUDED

/**
 * @file qh.h
 *
 * QNX Helpers - File initialization header file helper.
 *
 * Once <qh/qh.h> is included, QH_FILENAME is then made available to the source file and contains
 * the file name as reported by __FILE__, though with the path part stripped out.  In cases where
 * the used compiler isn't a GNU one, QH_FILENAME will simply contain __FILE__.  For this reason,
 * there QH_FILENAME_HAS_FULLPATH contains either true or false (though qh.h doesn't #include
 * stdbool.h) depending on if QH_FILENAME contains the full path or just the name of the file.  Note
 * also that QH_SHORT_FILENAME_DEFINED is also defined when QH_FILENAME_HAS_FULLPATH is false.
 *
 * The qh.h header also prevents the use of typically unsafe functions (strtok, strcpy, strcat,
 * strncpy, strncat, ...) and propose better alternatives at compile time.  If a client of the QH
 * library insists on allowing use of these functions, QH_ALLOW_UNSAFE (unsafe functions) and / or
 * QH_BYPASS_CS_RULES (unsafe functions prohibited by the QNX coding standard) must be defined prior
 * to including any of the QH header files.
 */

/* sorted headers from this project */

/* sorted library headers */
#if !defined(__QNXNTO__) && !defined(_LIB_COMPAT_H_INCLUDED)
#include <lib/compat.h>
#endif

/* sorted system headers */
#ifndef _ERRNO_H_INCLUDED
#include <errno.h>
#endif

#ifndef _STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef _STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef _STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef __CDEFS_H_INCLUDED
#include <sys/cdefs.h>
#endif

#ifndef _UNISTD_H_INCLUDED
#include <unistd.h>
#endif

/** @cond */
__BEGIN_DECLS
/** @endcond */

#if defined(__GNUC__)
#define QH_FILENAME (__builtin_strrchr(__FILE__, '/') != NULL) ? \
                        (__builtin_strrchr(__FILE__, '/') + 1) :  \
                        ((__builtin_strrchr(__FILE__, '\\') != NULL) ? \
                            (__builtin_strrchr(__FILE__, '\\') + 1) : \
                            __FILE__)
#define QH_FILENAME_HAS_FULLPATH                                                               false
#define QH_SHORT_FILENAME_DEFINED
#else
#define QH_FILENAME __FILE__
#define QH_FILENAME_HAS_FULLPATH                                                                true
#endif

/*
 * OS based support definitions
 */
#ifdef __QNXNTO__
#define QH_HAS_SUPPORT_SLOG
#define QH_HAS_SUPPORT_TIME_FUNC
#define QH_HAS_SUPPORT_STOPWATCH_FUNC
#endif

#if !defined(__MINGW32__) && !defined(__MINGW64__) && !defined(__DARWIN__)
#define QH_HAS_SUPPORT_QUICK_EXIT
#endif

/*
 * Block that checks for unsafe functions.
 */

#ifndef QH_ALLOW_UNSAFE

#undef strtok
#define strtok "strtok usage unsafe.  Use strtok_r instead."

#undef strcpy
#define strcpy "strcpy usage unsafe.  Use strlcpy instead."

#undef strcat
#define strcat "strcat usage unsafe.  Use strlcat instead."

#if !defined(__MINGW32__) && !defined(__MINGW64__)
/* Unfortunately, MinGW's io.h file makes use of strncpy in a header defined function. */
#undef strncpy
#define strncpy "strncpy usage unsafe.  Use strlcpy instead."
#endif

#undef strncat
#define strncat "strncat usage unsafe.  Use strlcat instead."

#undef gets
#define gets "gets usage unsafe. Use fgets instead."

#undef mktemp
#define mktemp "mktemp usage unsafe.  Use mkstemp or mkdtemp instead."

#endif /* QH_ALLOW_UNSAFE */

#ifndef QH_BYPASS_CS_RULES

#undef atoi
#define atoi "QNX Coding Standard rule - atoi usage prohibited (unsafe).  Use strtol instead."

#undef atol
#define atol "QNX Coding Standard rule - atol usage prohibited (unsafe).  Use strtol instead."

#undef atoll
#define atoll "QNX Coding Standard rule - atoll usage prohibited (unsafe).  Use strtoll instead."

#undef atof
#define atof "QNX Coding Standard rule - atof usage prohibited (unsafe).  Use strtod, strtof or strtold instead."

#endif /* QH_BYPASS_CS_RULES */

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QH_QH_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qh/public/qh/qh.h $ $Rev: 897545 $")
#endif
