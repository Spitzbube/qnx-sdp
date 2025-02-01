/*
 * Copyright (c) 2019, QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software. Free development
 * licenses are available for evaluation and non-commercial purposes. For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others. Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 */

#ifndef _FORTIFY_H_INCLUDED
#define _FORTIFY_H_INCLUDED

#include <sys/cdefs.h>


__BEGIN_DECLS


extern void __fortify_fail_extra_arg(void) __attribute__((__noreturn__));
extern void __fortify_fail_missing_arg(void) __attribute__((__noreturn__));
extern void __fortify_fail_overflow_dst(void) __attribute__((__noreturn__));

#define __FORTIFY_DIAG_PREFIX                    "_FORTIFY_SOURCE: "

#define __FORTIFY_ERROR_ATTRIBUTE(msg)           __ERROR_ATTRIBUTE(__FORTIFY_DIAG_PREFIX msg)
#define __FORTIFY_WARNING_ATTRIBUTE(msg)         __WARNING_ATTRIBUTE(__FORTIFY_DIAG_PREFIX msg)

/* If _FORTIFY_SOURCE_WARNINGS_ONLY is defined as a nonzero value, emit warnings in place of errors for function calls that are
 * known at compile time to always be unsafe/incorrect. Such function calls will each be replaced with an unconditional call to the
 * appropriate __fortify_fail_* function. This feature can be used to obtain a complete list of _FORTIFY_SOURCE compile-time
 * diagnostics for a given codebase. */
#if defined(_FORTIFY_SOURCE_WARNINGS_ONLY) && ( _FORTIFY_SOURCE_WARNINGS_ONLY != 0 )
#define __FORTIFY_FAIL_DIAG_ATTRIBUTE(msg)       __FORTIFY_WARNING_ATTRIBUTE(msg)
#else
#define __FORTIFY_FAIL_DIAG_ATTRIBUTE(msg)       __FORTIFY_ERROR_ATTRIBUTE(msg)
#endif

/* Macro that declares an alias of one of the __fortify_fail_* functions with an associated compile-time diagnostic message. When
 * the compiler encounters a call to the declared function that can't be optimized away, it emits the diagnostic message. */
#define __FORTIFY_FAIL_ALIAS_DECL(func_name, aliased_func_name, diag_msg)  \
        extern void __ALIAS(func_name, (void), #aliased_func_name) \
                __attribute__((__noreturn__)) \
                __FORTIFY_FAIL_DIAG_ATTRIBUTE(diag_msg)


__END_DECLS

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/c/public/fortify.h $ $Rev: 897587 $")
#endif
