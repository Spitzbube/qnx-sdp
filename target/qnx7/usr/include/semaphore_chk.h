/*
 * Copyright 2019, QNX Software Systems. All Rights Reserved.
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
 */

#ifndef _SEMAPHORE_H_INCLUDED
#error <semaphore_chk.h> should not be included directly.
#endif

#ifndef _SEMAPHORE_CHK_H_INCLUDED
#define _SEMAPHORE_CHK_H_INCLUDED

#include <fortify.h>


__BEGIN_DECLS


#ifdef __bvap
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_extra_arg_diag_sem_open,
        __fortify_fail_extra_arg,
        "more than 4 arguments specified to 'sem_open'");
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_missing_arg_diag_sem_open,
        __fortify_fail_missing_arg,
        "argument 2 of 'sem_open' has O_CREAT flag set, but fewer than 4 arguments specified");
extern sem_t *__ALIAS(__sem_open_alias, (const char *__name, int __oflag, ...), "sem_open");


extern __FORTIFY_FUNCTION sem_t *sem_open(const char *__name, int __oflag, ...)
{
    /* Fail if the caller specified more than the maximum number of arguments recognized by the sem_open function, as the caller may
     * have specified the __mode and/or __value argument in the wrong place */
    if ( __bvap_len > 2 ) {
        __fortify_fail_extra_arg_diag_sem_open();
    /* When the __oflag argument includes O_CREAT, additional __mode and __value arguments are required. Fail if the caller didn't
     * specify them. Also emit an error/warning if the __oflag argument is known to be constant at compile time. */
    } else if ( __bvap_len < 2 ) {
        if ( __bconst(__oflag) ) {
            if ( (__oflag & O_CREAT) != 0 ) {
                __fortify_fail_missing_arg_diag_sem_open();
            }
        } else {
            if ( (__oflag & O_CREAT) != 0 ) {
                __fortify_fail_missing_arg();
            }
        }
    } else {
        /* __mode and __value arguments were specified. They're required when the __oflag argument includes O_CREAT and ignored
         * otherwise. */
    }
    return __sem_open_alias(__name, __oflag, __bvap);
}
#endif /* #ifdef __bvap */


__END_DECLS

#endif /* #ifndef _SEMAPHORE_CHK_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/c/public/semaphore_chk.h $ $Rev: 903365 $")
#endif
