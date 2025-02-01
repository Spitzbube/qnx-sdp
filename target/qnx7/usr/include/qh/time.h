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

#ifndef _QH_TIME_H_INCLUDED
#define _QH_TIME_H_INCLUDED

/**
 * @file time.h
 *
 * QNX Helpers - Time related helpers.
 */

/* sorted headers from this project */
#ifndef _QH_QH_H_INCLUDED
#include "qh/qh.h"
#endif

/* sorted library headers */

/* sorted system headers */
#ifndef _STDBOOL_H_INCLUDED
#include <stdbool.h>
#endif

#ifndef _STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef _TIME_H_INCLUDED
#include <time.h>
#endif

/** @cond */
__BEGIN_DECLS
/** @endcond */

/**
 * Identifier that represents an unlimited sleep time.
 *
 * @see qh_nssleep
 */
#define QH_SLEEP_TIME_FOREVER                                                   (uint64_t)UINT64_MAX

/**
 * Identifier that represents infinity as an absolute time.
 *
 * @see qh_nssleep & qh_poll_until
 */
#define QH_ABSOLUTE_TIME_INFINITY                                               (uint64_t)UINT64_MAX

/** Unit conversion definitions, all in uint64_t. */
#define QH_NS_PER_US                                                                  UINT64_C(1000)

#define QH_US_PER_MS                                                                  UINT64_C(1000)
#define QH_NS_PER_MS                                                   (QH_US_PER_MS * QH_NS_PER_US)

#define QH_MS_PER_S                                                                   UINT64_C(1000)
#define QH_US_PER_S                                                     (QH_MS_PER_S * QH_US_PER_MS)
#define QH_NS_PER_S                                                     (QH_US_PER_S * QH_NS_PER_US)

#define QH_S_PER_MIN                                                                    UINT64_C(60)
#define QH_MS_PER_MIN                                                   (QH_S_PER_MIN * QH_MS_PER_S)
#define QH_US_PER_MIN                                                 (QH_MS_PER_MIN * QH_US_PER_MS)
#define QH_NS_PER_MIN                                                 (QH_US_PER_MIN * QH_NS_PER_US)

#define QH_MINS_PER_HOUR                                                                UINT64_C(60)
#define QH_S_PER_HOUR                                              (QH_MINS_PER_HOUR * QH_S_PER_MIN)
#define QH_MS_PER_HOUR                                                 (QH_S_PER_HOUR * QH_MS_PER_S)
#define QH_US_PER_HOUR                                               (QH_MS_PER_HOUR * QH_US_PER_MS)
#define QH_NS_PER_HOUR                                               (QH_US_PER_HOUR * QH_NS_PER_US)

#define QH_HOURS_PER_DAY                                                                UINT64_C(24)
#define QH_MINS_PER_DAY                                        (QH_HOURS_PER_DAY * QH_MINS_PER_HOUR)
#define QH_S_PER_DAY                                                (QH_MINS_PER_DAY * QH_S_PER_MIN)
#define QH_MS_PER_DAY                                                   (QH_S_PER_DAY * QH_MS_PER_S)
#define QH_US_PER_DAY                                                 (QH_MS_PER_DAY * QH_US_PER_MS)
#define QH_NS_PER_DAY                                                 (QH_US_PER_DAY * QH_NS_PER_US)

#define QH_DAYS_PER_WEEK                                                                 UINT64_C(7)
#define QH_HOURS_PER_WEEK                                      (QH_DAYS_PER_WEEK * QH_HOURS_PER_DAY)
#define QH_MINS_PER_WEEK                                      (QH_HOURS_PER_WEEK * QH_MINS_PER_HOUR)
#define QH_S_PER_WEEK                                              (QH_MINS_PER_WEEK * QH_S_PER_MIN)
#define QH_MS_PER_WEEK                                                 (QH_S_PER_WEEK * QH_MS_PER_S)
#define QH_US_PER_WEEK                                               (QH_MS_PER_WEEK * QH_US_PER_MS)
#define QH_NS_PER_WEEK                                               (QH_US_PER_WEEK * QH_NS_PER_US)

#ifdef QH_HAS_SUPPORT_TIME_FUNC

/**
 * Callback function type providing the results of a poll check.
 *
 * @param[in]  cb_data          The callback data provided in the @a cb_data parameter to
 *                              qh_poll_until().
 * @param[out] continue_polling Tells the qh_poll_until() function whether to continue polling.
 *
 * @retval EOK on success, indicating the poll executed correctly.  @a continue_polling then
 *         determines if polling should continue or not.
 * @retval A standard errno on failure, which is used as is as the return value of qh_poll_until().
 *
 * @see qh_poll_until
 */
typedef int qh_poll_check_cb_ft(void *__cb_data, bool *__continue_polling);

/**
 * Gets the (adjusted) time from the clock specified by @a clock_id in nanoseconds.
 *
 * @param[in] clock_id          The ID of the clock to get the time from.
 * @param[in] forward_offset_ns The number of nanoseconds by which to forward adjust the time.
 *
 * @return The time from the clock specified by @a clock_id in nanoseconds, or zero if an error
 *         occurs, in which case errno is set.
 */
uint64_t qh_get_nstime(clockid_t __clock_id, uint64_t __forward_offset_ns);

/**
 * Sleeps for a specified amount of time, trying not to pass a specified absolute time.  Sleeping is
 * done using the clock specified by @a clock_id.
 *
 * @param[in] sleep_time_ns        The amount of time to sleep for in nanoseconds.  Setting this to
 *                                 0 returns right away, either with EOK, ETIME or a standard errno
 *                                 value.
 * @param[in] clock_id             The ID of the clock to use for the operation.
 * @param[in] max_absolute_time_ns The maximum absolute time (in nanoseconds) at which sleeping
 *                                 should stop in the case where @a sleep_time_ns would go beyond it
 *                                 (in which case the sleep time ends up being shortened).  This
 *                                 time is based on the clock specified by @a clock_id.
 *
 * @return EOK, ETIME or a standard errno on failure.  errno is set if the return value is not EOK.
 *
 * @retval EOK if the function slept for the amount of time specified by @a sleep_time_ns and has
 *         not reached the specified absolute time.
 * @retval ETIME if the time specified by @a max_absolute_time_ns has passed.  The function may or
 *         may not have slept before returning.
 */
int qh_nssleep(uint64_t __sleep_time_ns, clockid_t __clock_id, uint64_t __max_absolute_time_ns);

/**
 * Calls the poll check function @a poll_check_f at an interval of @a poll_interval_ns nanoseconds
 * until a maximum absolute time has been reached, or the poll check function tells it to stop
 * polling.  Sleeping between the poll check calls and evaluating the maximum absolute time is done
 * using the clock specified by @a clock_id.
 *
 * @param[in] poll_interval_ns     The amount of time (in nanoseconds) between the poll check calls.
 * @param[in] clock_id             The ID of the clock to use for the operation.
 * @param[in] max_absolute_time_ns The maximum absolute time (in nanoseconds) at which polling
 *                                 should stop, based on the clock specified by @a clock_id.
 * @param[in] poll_check_f         The poll function used to determine if polling should continue or
 *                                 not.
 * @param[in] cb_data              The callback data provided to the @a cb_data parameter of
 *                                 @a poll_check_f.
 *
 * @return EOK, ETIME or a standard errno on failure.  errno is set if the return value is not EOK.
 *
 * @retval EOK if the function slept for the amount of time specified by @a sleep_time_ns and has
 *         not reached the specified absolute time.
 * @retval ETIME if the time specified by @a max_absolute_time_ns has passed.  The function may or
 *         may not have slept before returning.
 *
 * @see qh_poll_check_cb_ft
 */
int qh_poll_until(uint64_t __poll_interval_ns, clockid_t __clock_id,
                  uint64_t __max_absolute_time_ns, qh_poll_check_cb_ft __poll_check_f,
                  void *__cb_data);

#endif /* QH_HAS_SUPPORT_TIME_FUNC */

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QH_TIME_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qh/public/qh/time.h $ $Rev: 904095 $")
#endif
