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

#ifndef _MQUEUE_H_INCLUDED
#error <mqueue_chk.h> should not be included directly.
#endif

#ifndef _MQUEUE_CHK_H_INCLUDED
#define _MQUEUE_CHK_H_INCLUDED

#include <fortify.h>


__BEGIN_DECLS


#ifdef __bvap
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_extra_arg_diag_mq_open,
        __fortify_fail_extra_arg,
        "more than 4 arguments specified to 'mq_open'");
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_missing_arg_diag_mq_open,
        __fortify_fail_missing_arg,
        "argument 2 of 'mq_open' has O_CREAT flag set, but fewer than 4 arguments specified");
extern mqd_t __ALIAS(__mq_open_alias, (const char *__name, int __oflag, ...), "mq_open");
#endif

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_mq_receive,
        __fortify_fail_overflow_dst,
        "argument 3 of 'mq_receive' is greater than the length of the object referenced by argument 2");
extern ssize_t __ALIAS(__mq_receive_alias, (mqd_t __mqdes, char *__msg_ptr, size_t __msg_len, unsigned *__msg_prio), "mq_receive");

#if defined(__EXT_POSIX1_200112)
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_mq_timedreceive,
        __fortify_fail_overflow_dst,
        "argument 3 of 'mq_timedreceive' is greater than the length of the object referenced by argument 2");
extern ssize_t __ALIAS(
        __mq_timedreceive_alias,
        (mqd_t __mqdes, char *__msg_ptr, size_t __msg_len, unsigned *__msg_prio, const struct timespec *__abs_timeout),
        "mq_timedreceive");
#endif /* #if defined(__EXT_POSIX1_200112) */

#ifdef __EXT_QNX
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_mq_timedreceive_monotonic,
        __fortify_fail_overflow_dst,
        "argument 3 of 'mq_timedreceive_monotonic' is greater than the length of the object referenced by argument 2");
extern ssize_t __ALIAS(
        __mq_timedreceive_monotonic_alias,
        (mqd_t __mqdes, char *__msg_ptr, size_t __msg_len, unsigned *__msg_prio, const struct timespec *__abs_mono_timeout),
        "mq_timedreceive_monotonic");
#endif /* #ifdef __EXT_QNX */


#ifdef __bvap
extern __FORTIFY_FUNCTION mqd_t mq_open(const char *__name, int __oflag, ...)
{
    /* Fail if the caller specified more than the maximum number of arguments recognized by the mq_open function, as the caller may
     * have specified the __mode and/or __mq_attr argument in the wrong place */
    if ( __bvap_len > 2 ) {
        __fortify_fail_extra_arg_diag_mq_open();
    /* When the __oflag argument includes O_CREAT, additional __mode and __mq_attr arguments are required. Fail if the caller didn't
     * specify them. Also emit an error/warning if the __oflag argument is known to be constant at compile time. */
    } else if ( __bvap_len < 2 ) {
        if ( __bconst(__oflag) ) {
            if ( (__oflag & O_CREAT) != 0 ) {
                __fortify_fail_missing_arg_diag_mq_open();
            }
        } else {
            if ( (__oflag & O_CREAT) != 0 ) {
                __fortify_fail_missing_arg();
            }
        }
    } else {
        /* __mode and __mq_attr arguments were specified. They're required when the __oflag argument includes O_CREAT and ignored
         * otherwise. */
    }
    return __mq_open_alias(__name, __oflag, __bvap);
}
#endif /* #ifdef __bvap */

extern __FORTIFY_FUNCTION ssize_t mq_receive(mqd_t __mqdes, char *__msg_ptr, size_t __msg_len, unsigned *__msg_prio)
{
    /* Fail if the maximum number of bytes to be received (__msg_len) is greater than the length of the destination object
     * (__msg_ptr) determined at compile time. Also emit an error/warning if the __msg_len argument is known to be constant at
     * compile time. __bos0 is used to obtain the length of the destination object instead of __bos in order to reduce false
     * positives when the _FORTIFY_SOURCE feature test macro has been defined with a value greater than 1. */
    if ( __bos0(__msg_ptr) != (size_t)-1 ) {
        if ( __bconst(__msg_len) ) {
            if ( __unlikely(__msg_len > __bos0(__msg_ptr)) ) {
                __fortify_fail_overflow_dst_diag_mq_receive();
            }
        } else {
            if ( __unlikely(__msg_len > __bos0(__msg_ptr)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    return __mq_receive_alias(__mqdes, __msg_ptr, __msg_len, __msg_prio);
}

#if defined(__EXT_POSIX1_200112)
extern __FORTIFY_FUNCTION ssize_t mq_timedreceive(
        mqd_t                  __mqdes,
        char                  *__msg_ptr,
        size_t                 __msg_len,
        unsigned              *__msg_prio,
        const struct timespec *__abs_timeout)
{
    /* Fail if the maximum number of bytes to be received (__msg_len) is greater than the length of the destination object
     * (__msg_ptr) determined at compile time. Also emit an error/warning if the __msg_len argument is known to be constant at
     * compile time. __bos0 is used to obtain the length of the destination object instead of __bos in order to reduce false
     * positives when the _FORTIFY_SOURCE feature test macro has been defined with a value greater than 1. */
    if ( __bos0(__msg_ptr) != (size_t)-1 ) {
        if ( __bconst(__msg_len) ) {
            if ( __unlikely(__msg_len > __bos0(__msg_ptr)) ) {
                __fortify_fail_overflow_dst_diag_mq_timedreceive();
            }
        } else {
            if ( __unlikely(__msg_len > __bos0(__msg_ptr)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    return __mq_timedreceive_alias(__mqdes, __msg_ptr, __msg_len, __msg_prio, __abs_timeout);
}
#endif /* #if defined(__EXT_POSIX1_200112) */

#ifdef __EXT_QNX
extern __FORTIFY_FUNCTION ssize_t mq_timedreceive_monotonic(
        mqd_t __mqdes,
        char *__msg_ptr,
        size_t __msg_len,
        unsigned *__msg_prio,
        const struct timespec *__abs_mono_timeout)
{
    /* Fail if the maximum number of bytes to be received (__msg_len) is greater than the length of the destination object
     * (__msg_ptr) determined at compile time. Also emit an error/warning if the __msg_len argument is known to be constant at
     * compile time. __bos0 is used to obtain the length of the destination object instead of __bos in order to reduce false
     * positives when the _FORTIFY_SOURCE feature test macro has been defined with a value greater than 1. */
    if ( __bos0(__msg_ptr) != (size_t)-1 ) {
        if ( __bconst(__msg_len) ) {
            if ( __unlikely(__msg_len > __bos0(__msg_ptr)) ) {
                __fortify_fail_overflow_dst_diag_mq_timedreceive_monotonic();
            }
        } else {
            if ( __unlikely(__msg_len > __bos0(__msg_ptr)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    return __mq_timedreceive_monotonic_alias(__mqdes, __msg_ptr, __msg_len, __msg_prio, __abs_mono_timeout);
}
#endif /* #ifdef __EXT_QNX */


__END_DECLS

#endif /* #ifndef _MQUEUE_CHK_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/c/public/mqueue_chk.h $ $Rev: 903365 $")
#endif
