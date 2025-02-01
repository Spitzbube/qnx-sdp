/*
 * $QNXLicenseC:
 * Copyright 2011-2018, QNX Software Systems. All Rights Reserved.
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


/* Description: library interface for adding entropy to random */

#ifndef _ENTROPY_H_INCLUDED
#define _ENTROPY_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include <sys/neutrino.h>

typedef enum {
    ENTROPY_SUCCESS = 0,
    ENTROPY_FAIL,
    ENTROPY_FAIL_BAD_PARAMETERS,
    ENTROPY_FAIL_NOT_REGISTERED,
    ENTROPY_FAIL_THREAD_CREATION,
} entropy_return_status;

static inline void entropy_sleep(unsigned int const seconds)
{
    uint64_t const nsec = (uint64_t)seconds * 1000000000u;
    TimerTimeout(CLOCK_SOFTTIME, _NTO_TIMEOUT_NANOSLEEP, NULL, &nsec, NULL);
}

/**
 * Prototype for function to be used by library entropy sources to add entropy to random.  While
 * the quality and size of the entropy being added is largely arbitrary, large volumes of low
 * quality entropy should not be added directly but rather hashed using a cryptographic hash
 * and the resultant hash added instead.
 *
 * The first call to this function is guaranteed to be used as part of the initial seeding of
 * the PRNG and thus may be used to ensure the quality of data provided by random on startup.
 *
 * This function must be called within 5 seconds of random_source_init being called to avoid a
 * startup failure of random.  If entropy is not immediately available, one may make a call with a
 * size of zero.  In all cases the initial call to random_add_entropy_fn should be delayed until the
 * library has completed all privileged operations that are needed only during start-up.
 *
 * This function is thread safe.
 *
 * @param id        Identifier for entropy source.  This must be the value passed to random_source_init.
 * @param buffer    Pointer to buffer containing entropy to be added.  This may be NULL if size
 *                  is zero.
 * @param size      Size of buffer in bytes
 */
typedef void (*random_add_entropy_fn)(unsigned int id, void const *buffer, uint32_t size);


 /**
 * Prototype for library entropy source initialization function.  To use the new random entropy
 * library interface a dll must export a function with this signature called "random_source_init".
 * It is the first, and possibly only function called in the entropy dll.
 *
 * As this function must return, the library is expected to create its own thread for adding
 * entropy.
 *
 * @param id        Identifier for entropy source.  This must be provided when adding entropy.
 * @param paddfn    Function for library to call to add entropy.  This is expected to be called
 *                  periodically and must be called within 5s of random_source_init being called .
 * @param context   A pointer to a character string providing options for the dll.  Options may
 *                  be provided by following the library name with ':option-string' when using
 *                  random's -l option.  If no option string was provided, this is an empty
 *                  string.
 * @return ENTROPY_SUCCESS if successful
 */
typedef entropy_return_status (*random_source_init_fn)(unsigned int id, random_add_entropy_fn paddfn, const char *context);

 /**
 * Prototype for library entropy source stop function.  If an entropy source requires notification
 * of when random is about to exit, it should provide a function with this signature called
 * "random_source_stop".
 */
typedef void (*random_source_stop_fn)(void);

#endif /* !_ENTROPY_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/services/random/public/entropy.h $ $Rev: 878632 $")
#endif
