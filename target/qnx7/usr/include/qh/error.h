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

#ifndef _QH_ERROR_H_INCLUDED
#define _QH_ERROR_H_INCLUDED

/**
 * @file error.h
 *
 * QNX Helpers - Error related helpers.
 */

/* sorted headers from this project */
#ifndef _QH_LOG_H_INCLUDED
#include "qh/log.h"
#endif

/* sorted library headers */

/* sorted system headers */

/** @cond */
__BEGIN_DECLS
/** @endcond */

/**
 * Macro that sets errno.
 *
 * Note that what is passed in to the macro will only be evaluated once, and so it is safe to have
 * operations being executed as part of calling the macro.
 *
 * @param[in] __errno The standard errno to set errno to.
 */
#define QH_SET_ERRNO(__errno) (errno = (__errno))

/**
 * Macro that sets errno and returns its value.
 *
 * Given this is a macro that calls return, the invoking function will actually return when the
 * macro is executed.  The invoking function is expected to have an int as its return type.
 *
 * Note that what is passed in to the macro will only be evaluated once, and so it is safe to have
 * operations being executed as part of calling the macro.
 *
 * @param[in] __errno The standard errno to set errno to and to then return with.
 */
#define QH_RET_ERRNO(__errno) \
            do { \
                errno = (__errno); \
                return errno; \
            } while (false)

/**
 * Macro that conditionally sets errno and always returns the passed in error.
 *
 * If the passed in __errno is not EOK, errno is set to it, and then the pre-evaluated value of
 * __errno is returned.  If __errno evaluates to EOK, errno is not set, but EOK is the returned.
 *
 * Given this is a macro that calls return, the invoking function will actually return when the
 * macro is executed.  The invoking function is expected to have an int as its return type.
 *
 * Note that what is passed in to the macro will only be evaluated once, and so it is safe to have
 * operations being executed as part of calling the macro.
 *
 * @param[in] __errno The standard errno to set errno to (if not EOK) and to then return with.
 */
#define QH_RET_ERRNO_WEOK(__errno) \
            do { \
                const int qh_ret_errno_weok_errno__ = (__errno); \
                if (qh_ret_errno_weok_errno__ != EOK) { \
                    errno = qh_ret_errno_weok_errno__; \
                    return qh_ret_errno_weok_errno__; \
                } \
                return EOK; \
            } while (false)

/**
 * Macro that conditionally (if not EOK) sets errno and returns the passed in error.
 *
 * If the passed in __errno is not EOK, errno is set to it, and then the pre-evaluated value of
 * __errno is returned.  If __errno evaluates to EOK, errno is not set and the macro does not call
 * return.
 *
 * Given this is a macro that conditionally calls return, the invoking function may actually return
 * when the macro is executed.  The invoking function is expected to have an int as its return type.
 *
 * Note that what is passed in to the macro will only be evaluated once, and so it is safe to have
 * operations being executed as part of calling the macro.
 *
 * @param[in] __errno The standard errno to set errno to (if not EOK) and to then return with.
 */
#define QH_RET_ON_ERRNO(__errno) \
        do { \
            const int qh_ret_on_errno_errno__ = (__errno); \
            if (qh_ret_on_errno_errno__ != EOK) { \
                errno = qh_ret_on_errno_errno__; \
                return qh_ret_on_errno_errno__; \
            } \
        } while (false)

/**
 * Convenience macro that calls qh_abort_on_errno, but that doesn't require specifying the
 * qh_log_info_t parameter.  The structure is defined for the caller with a few default parameters
 * that will make the log message show the originating location of the abort call in case of
 * failure.
 *
 * @param[in] __errno     The standard errno to validate (and log in case of failure).  Passing a
 *                        value smaller than EOK / 0 logs an error without details about the errno
 *                        and aborts.
 * @param[in] err_msg     The message to be logged prior to aborting in case of failure.
 *
 * @return EOK on success or a standard errno on failure.  errno is unmodified.
 */
#define QH_ABORT_ON_ERRNO(__errno, __err_msg) \
        qh_abort_on_errno(QH_LOG_INFO_ANON(QH_LOG_CRITICAL, QH_FILENAME, __func__, __LINE__), __errno, __err_msg)

/**
 * Conditionally logs an error message using qh_log and then calls abort if the passed in __errno is
 * not EOK.
 *
 * If the passed in __errno is not EOK, the function logs an error message, and then calls abort().
 * If the passed in __errno is EOK, the function does nothing.
 *
 * @param[in] qh_log_info The logging information for the abort message.
 * @param[in] __errno     The standard errno to validate (and log in case of failure).  Passing a
 *                        value smaller than EOK / 0 logs an error without details about the errno
 *                        and aborts.
 * @param[in] err_msg     The message to be logged prior to aborting in case of failure.
 *
 * @return None
 */
void qh_abort_on_errno(const qh_log_info_t *__qh_log_info, int __errno, const char *__err_msg);

/**
 * Convenience macro that logs an error message using qh_log and then calls abort.
 *
 * @param[in] err_fmt     A string that specifies the message to be logged prior to aborting.  The
 *                        formatting string determines what additional arguments you need to
 *                        provide.  For more information, see printf().
 *
 * @return This function does not return.
 */
#define QH_ABORT(...) \
            do { \
                qh_log_ex(QH_LOG_INFO_ANON(QH_LOG_CRITICAL, QH_FILENAME, __func__, __LINE__), ##__VA_ARGS__); \
                abort(); \
            } while (false)

/**
 * Stores a new error code in a sticky location if the new error is not zero and the sticky one is.
 *
 * This allows a caller to make successive calls with the same sticky_error location and with
 * potentially different new_error, and at the end look at the value found in sticky_error to
 * determine what the first error was in the successive calls, if there was one.
 *
 * This can be used with standard error numbers (e.g. errno), but also with any set of error codes
 * for which what is considered a success value has value zero, which is often the case with enums.
 *
 * @param[in,out] sticky_error The sticky error that stores the first encountered error code in
 *                             successive calls to the function.
 * @param[in]     new_error    The new error code to be considered for the sticky error.
 *
 * @return The value of new_error.
 */
int qh_sticky_error(int *__sticky_error, int __new_error);

/**
 * Stores a new error code in a sticky location if the new error is not zero.
 *
 * This allows a caller to make successive calls with the same sticky_error location and with
 * potentially different new_error, and at the end look at the value found in sticky_error to
 * determine what the last error was in the successive calls, if there was one.
 *
 * @param[in,out] sticky_error The sticky error that stores the last encountered error code in
 *                             successive calls to the function.
 * @param[in]     new_error    The new error code to be considered for the sticky error.
 *
 * @return The value of new_error.
 */
int qh_sticky_error_last(int *__sticky_error, int __new_error);

/**
 * Convert an error number into an error message, and guarantee that errno is kept intact in the
 * process.
 *
 * This function simply makes use of strerror, but guarantees that errno is the same on the way out
 * of the function as it was on the way in.
 *
 * Note that the returned string should not be modified.
 *
 * @param[in] __errno The error number that you want the message for.
 *
 * @return A pointer to the error message.
 */
const char * qh_strerror(int __errno);

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QH_ERROR_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qh/public/qh/error.h $ $Rev: 897735 $")
#endif
