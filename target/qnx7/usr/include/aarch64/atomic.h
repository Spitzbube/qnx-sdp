/*
 * $QNXLicenseC:
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
 * $
 */

#ifndef _AARCH64_ATOMIC_H_INCLUDED
#define _AARCH64_ATOMIC_H_INCLUDED

#ifndef _ATOMIC_H_INCLUDED
#error aarch64/atomic.h should not be included directly.
#endif

/*
 * LLSC atomic set
 */

static __inline__ unsigned __attribute__((__unused__)) llsc_atomic_add_value(volatile unsigned * const __loc, unsigned const __incr)
{
    unsigned __res;
    unsigned __tmp1;
    unsigned __tmp2;
    unsigned __tmp3;

    __asm__ __volatile__(
        "   mov     %1, %4\n"
        "1: ldaxr   %w4, [%1]\n"
        "   add     %w2, %w4, %w5\n"
        "   stlxr   %w3, %w2, [%1]\n"
        "   cbnz    %w3, 1b\n"
        : "=&r" (__res), "=&r" (__tmp1), "=&r" (__tmp2), "=&r" (__tmp3)
        : "0" (__loc), "r" (__incr)
        : "cc", "memory"
    );

    return (__res);
}

static __inline__ unsigned __attribute__((__unused__)) llsc_atomic_clr_value(volatile unsigned * const __loc, unsigned const __bits)
{
    unsigned __res;
    unsigned __tmp1;
    unsigned __tmp2;
    unsigned __tmp3;

    __asm__ __volatile__(
        "   mov     %1, %4\n"
        "1: ldaxr   %w4, [%1]\n"
        "   bic     %w2, %w4, %w5\n"
        "   stlxr   %w3, %w2, [%1]\n"
        "   cbnz    %w3, 1b\n"
        : "=&r" (__res), "=&r" (__tmp1), "=&r" (__tmp2), "=&r" (__tmp3)
        : "0" (__loc), "r" (__bits)
        : "cc", "memory"
    );

    return (__res);
}

static __inline__ unsigned __attribute__((__unused__)) llsc_atomic_set_value(volatile unsigned * const __loc, unsigned const __bits)
{
    unsigned __res;
    unsigned __tmp1;
    unsigned __tmp2;
    unsigned __tmp3;

    __asm__ __volatile__(
        "   mov     %1, %4\n"
        "1: ldaxr   %w4, [%1]\n"
        "   orr     %w2, %w4, %w5\n"
        "   stlxr   %w3, %w2, [%1]\n"
        "   cbnz    %w3, 1b\n"
        : "=&r" (__res), "=&r" (__tmp1), "=&r" (__tmp2), "=&r" (__tmp3)
        : "0" (__loc), "r" (__bits)
        : "cc", "memory"
    );

    return (__res);
}

static __inline__ unsigned __attribute__((__unused__)) llsc_atomic_sub_value(volatile unsigned * const __loc, unsigned const __decr)
{
    unsigned __res;
    unsigned __tmp1;
    unsigned __tmp2;
    unsigned __tmp3;

    __asm__ __volatile__(
        "   mov     %1, %4\n"
        "1: ldaxr   %w4, [%1]\n"
        "   sub     %w2, %w4, %w5\n"
        "   stlxr   %w3, %w2, [%1]\n"
        "   cbnz    %w3, 1b\n"
        : "=&r" (__res), "=&r" (__tmp1), "=&r" (__tmp2), "=&r" (__tmp3)
        : "0" (__loc), "r" (__decr)
        : "cc", "memory"
    );

    return (__res);
}

static __inline__ unsigned __attribute__((__unused__)) llsc_atomic_toggle_value(volatile unsigned * const __loc, unsigned const __bits)
{
    unsigned __res;
    unsigned __tmp1;
    unsigned __tmp2;
    unsigned __tmp3;

    __asm__ __volatile__(
        "   mov     %1, %4\n"
        "1: ldaxr   %w4, [%1]\n"
        "   eor     %w2, %w4, %w5\n"
        "   stlxr   %w3, %w2, [%1]\n"
        "   cbnz    %w3, 1b\n"
        : "=&r" (__res), "=&r" (__tmp1), "=&r" (__tmp2), "=&r" (__tmp3)
        : "0" (__loc), "r" (__bits)
        : "cc", "memory"
    );

    return (__res);
}

static __inline__ void __attribute__((__unused__)) llsc_atomic_add(volatile unsigned * const __loc, unsigned const __incr)
{
	llsc_atomic_add_value(__loc, __incr);
}

static __inline__ void __attribute__((__unused__)) llsc_atomic_clr(volatile unsigned * const __loc, unsigned const __bits)
{
	llsc_atomic_clr_value(__loc, __bits);
}

static __inline__ void __attribute__((__unused__)) llsc_atomic_set(volatile unsigned * const __loc, unsigned const __bits)
{
	llsc_atomic_set_value(__loc, __bits);
}

static __inline__ void __attribute__((__unused__)) llsc_atomic_sub(volatile unsigned * const __loc, unsigned const __decr)
{
	llsc_atomic_sub_value(__loc, __decr);
}

static __inline__ void __attribute__((__unused__)) llsc_atomic_toggle(volatile unsigned * const __loc, unsigned const __bits)
{
	llsc_atomic_toggle_value(__loc, __bits);
}

/*
 * LSE atomic set
 */
static __inline__ unsigned __attribute__((__unused__)) lse_atomic_add_value(volatile unsigned * const __loc, unsigned const __incr)
{
	unsigned __res;

	__asm__ __volatile__ (
		".arch armv8.1-a\n"
		"ldaddal	%w2, %w0, [%1]\n"
		: "=r" (__res)
		: "0" (__loc), "r" (__incr)
		: "memory"
	);

	return (__res);
}

static __inline__ unsigned __attribute__((__unused__)) lse_atomic_clr_value(volatile unsigned * const __loc, unsigned const __bits)
{
	unsigned __res;

	__asm__ __volatile__ (
		".arch armv8.1-a\n"
		"ldclral	%w2, %w0, [%1]\n"
		: "=r" (__res)
		: "0" (__loc), "r" (__bits)
		: "memory"
	);

	return (__res);
}

static __inline__ unsigned __attribute__((__unused__)) lse_atomic_set_value(volatile unsigned * const __loc, unsigned const __bits)
{
	unsigned __res;

	__asm__ __volatile__ (
		".arch armv8.1-a\n"
		"ldsetal	%w2, %w0, [%1]\n"
		: "=r" (__res)
		: "0" (__loc), "r" (__bits)
		: "memory"
	);

	return (__res);
}

static __inline__ unsigned __attribute__((__unused__)) lse_atomic_sub_value(volatile unsigned * const __loc, unsigned const __decr)
{
	unsigned __res;

	__asm__ __volatile__ (
		".arch armv8.1-a\n"
		"neg		%w2, %w2\n"
		"ldaddal	%w2, %w0, [%1]\n"
		: "=r" (__res)
		: "0" (__loc), "r" (__decr)
		: "memory"
	);

	return (__res);
}

static __inline__ unsigned __attribute__((__unused__)) lse_atomic_toggle_value(volatile unsigned * const __loc, unsigned const __bits)
{
	unsigned __res;

	__asm__ __volatile__ (
		".arch armv8.1-a\n"
		"ldeoral	%w2, %w0, [%1]\n"
		: "=r" (__res)
		: "0" (__loc), "r" (__bits)
		: "memory"
	);

	return (__res);
}

static __inline__ void __attribute__((__unused__)) lse_atomic_add(volatile unsigned * const __loc, unsigned const __incr)
{
	lse_atomic_add_value(__loc, __incr);
}

static __inline__ void __attribute__((__unused__)) lse_atomic_clr(volatile unsigned * const __loc, unsigned const __bits)
{
	lse_atomic_clr_value(__loc, __bits);
}

static __inline__ void __attribute__((__unused__)) lse_atomic_set(volatile unsigned * const __loc, unsigned const __bits)
{
	lse_atomic_set_value(__loc, __bits);
}

static __inline__ void __attribute__((__unused__)) lse_atomic_sub(volatile unsigned * const __loc, unsigned const __decr)
{
	lse_atomic_sub_value(__loc, __decr);
}

static __inline__ void __attribute__((__unused__)) lse_atomic_toggle(volatile unsigned * const __loc, unsigned const __bits)
{
	lse_atomic_toggle_value(__loc, __bits);
}

#endif /* #ifndef _AARCH64_ATOMIC_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/c/public/aarch64/atomic.h $ $Rev: 905296 $")
#endif
