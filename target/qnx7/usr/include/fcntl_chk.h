/*
 * $QNXLicenseC:
 * Copyright 2018-2019, QNX Software Systems. All Rights Reserved.
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

#ifndef _FCNTL_H_INCLUDED
#error <fcntl_chk.h> should not be included directly.
#endif

#ifndef _FCNTL_CHK_H_INCLUDED
#define _FCNTL_CHK_H_INCLUDED

#include <fortify.h>


__BEGIN_DECLS


#ifdef __bvap

#ifdef __EXT_LF64SRC
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_extra_arg_diag_open64,
        __fortify_fail_extra_arg,
        "more than 3 arguments specified to 'open64'");
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_missing_arg_diag_open64,
        __fortify_fail_missing_arg,
        "argument 2 of 'open64' has O_CREAT flag set, but fewer than 3 arguments specified");
#ifdef __EXT_LF64ALIAS
extern int __ALIAS(__open64_alias, (const char *__path, int __oflag, ...), "open");
#else
extern int __ALIAS(__open64_alias, (const char *__path, int __oflag, ...), "open64");
#endif
#endif

#if defined(__EXT_POSIX1_198808)
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_extra_arg_diag_open,
        __fortify_fail_extra_arg,
        "more than 3 arguments specified to 'open'");
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_missing_arg_diag_open,
        __fortify_fail_missing_arg,
        "argument 2 of 'open' has O_CREAT flag set, but fewer than 3 arguments specified");
#ifdef __EXT_FUNCALIAS64
extern int __ALIAS(__open_alias, (const char *__path, int __oflag, ...), "open64");
#else
extern int __ALIAS(__open_alias, (const char *__path, int __oflag, ...), "open");
#endif
#endif

#if defined(__EXT_POSIX1_200809)
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_extra_arg_diag_openat,
        __fortify_fail_extra_arg,
        "more than 4 arguments specified to 'openat'");
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_missing_arg_diag_openat,
        __fortify_fail_missing_arg,
        "argument 3 of 'openat' has O_CREAT flag set, but fewer than 4 arguments specified");
extern int __ALIAS(__openat_alias, (int __fd, const char *__path, int __oflag, ...), "openat");
#endif

#if defined(__EXT_PCDOS)
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_extra_arg_diag_sopen,
        __fortify_fail_extra_arg,
        "more than 4 arguments specified to 'sopen'");
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_missing_arg_diag_sopen,
        __fortify_fail_missing_arg,
        "argument 2 of 'sopen' has O_CREAT flag set, but fewer than 4 arguments specified");
extern int __ALIAS(__sopen_alias, (const char *__path, int __oflag, int __share, ...), "sopen");
#endif


#ifdef __EXT_LF64SRC
extern __FORTIFY_FUNCTION int open64(const char *__path, int __oflag, ...)
{
    /* Fail if the caller specified more than the maximum number of arguments recognized by the open64 function, as the caller may
     * have specified the __mode argument in the wrong place */
    if ( __bvap_len > 1 ) {
        __fortify_fail_extra_arg_diag_open64();
    /* When the __oflag argument includes O_CREAT, an additional __mode argument is required. Fail if the caller didn't specify
     * it. Also emit an error/warning if the __oflag argument is known to be constant at compile time. */
    } else if ( __bvap_len < 1 ) {
        if ( __bconst(__oflag) ) {
            if ( (__oflag & O_CREAT) != 0 ) {
                __fortify_fail_missing_arg_diag_open64();
            }
        } else {
            if ( (__oflag & O_CREAT) != 0 ) {
                __fortify_fail_missing_arg();
            }
        }
    } else {
        /* A __mode argument was specified. It's required when the __oflag argument includes O_CREAT and ignored otherwise. */
    }
    return __open64_alias(__path, __oflag, __bvap);
}
#endif

#if defined(__EXT_POSIX1_198808)
extern __FORTIFY_FUNCTION int open(const char *__path, int __oflag, ...)
{
    /* Fail if the caller specified more than the maximum number of arguments recognized by the open function, as the caller may
     * have specified the __mode argument in the wrong place */
    if ( __bvap_len > 1 ) {
        __fortify_fail_extra_arg_diag_open();
    /* When the __oflag argument includes O_CREAT, an additional __mode argument is required. Fail if the caller didn't specify
     * it. Also emit an error/warning if the __oflag argument is known to be constant at compile time. */
    } else if ( __bvap_len < 1 ) {
        if ( __bconst(__oflag) ) {
            if ( (__oflag & O_CREAT) != 0 ) {
                __fortify_fail_missing_arg_diag_open();
            }
        } else {
            if ( (__oflag & O_CREAT) != 0 ) {
                __fortify_fail_missing_arg();
            }
        }
    } else {
        /* A __mode argument was specified. It's required when the __oflag argument includes O_CREAT and ignored otherwise. */
    }
    return __open_alias(__path, __oflag, __bvap);
}
#endif

#if defined(__EXT_POSIX1_200809)
extern __FORTIFY_FUNCTION int openat(int __fd, const char *__path, int __oflag, ...)
{
    /* Fail if the caller specified more than the maximum number of arguments recognized by the openat function, as the caller may
     * have specified the __mode argument in the wrong place */
    if ( __bvap_len > 1 ) {
        __fortify_fail_extra_arg_diag_openat();
    /* When the __oflag argument includes O_CREAT, an additional __mode argument is required. Fail if the caller didn't specify
     * it. Also emit an error/warning if the __oflag argument is known to be constant at compile time. */
    } else if ( __bvap_len < 1 ) {
        if ( __bconst(__oflag) ) {
            if ( (__oflag & O_CREAT) != 0 ) {
                __fortify_fail_missing_arg_diag_openat();
            }
        } else {
            if ( (__oflag & O_CREAT) != 0 ) {
                __fortify_fail_missing_arg();
            }
        }
    } else {
        /* A __mode argument was specified. It's required when the __oflag argument includes O_CREAT and ignored otherwise. */
    }
    return __openat_alias(__fd, __path, __oflag, __bvap);
}
#endif

#if defined(__EXT_PCDOS)
extern __FORTIFY_FUNCTION int sopen(const char *__path, int __oflag, int __share, ...)
{
    /* Fail if the caller specified more than the maximum number of arguments recognized by the sopen function, as the caller may
     * have specified the __mode argument in the wrong place */
    if ( __bvap_len > 1 ) {
        __fortify_fail_extra_arg_diag_sopen();
    /* When the __oflag argument includes O_CREAT, an additional __mode argument is required. Fail if the caller didn't specify
     * it. Also emit an error/warning if the __oflag argument is known to be constant at compile time. */
    } else if ( __bvap_len < 1 ) {
        if ( __bconst(__oflag) ) {
            if ( (__oflag & O_CREAT) != 0 ) {
                __fortify_fail_missing_arg_diag_sopen();
            }
        } else {
            if ( (__oflag & O_CREAT) != 0 ) {
                __fortify_fail_missing_arg();
            }
        }
    } else {
        /* A __mode argument was specified. It's required when the __oflag argument includes O_CREAT and ignored otherwise. */
    }
    return __sopen_alias(__path, __oflag, __share, __bvap);
}
#endif

#endif /* #ifdef __bvap */


__END_DECLS

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/c/public/fcntl_chk.h $ $Rev: 903365 $")
#endif
