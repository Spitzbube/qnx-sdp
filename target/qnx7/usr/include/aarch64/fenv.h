/*-
 * Copyright (c) 2004-2005 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#ifndef	_FENV_H_
#define	_FENV_H_

#include <yvals.h>

#ifndef	__fenv_static
#define	__fenv_static	static
#endif

#ifdef __QNXNTO__
typedef struct {
	_Uint32t	__control;
	_Uint32t	__status;
} fenv_t;
typedef	_Uint32t	fexcept_t;
#else
typedef	_Uint64t	fenv_t;
typedef	_Uint64t	fexcept_t;
#endif

/* Exception flags */
#ifdef __QNXNTO__
#define	FE_INVALID	0x00000001U
#define	FE_DIVBYZERO	0x00000002U
#define	FE_OVERFLOW	0x00000004U
#define	FE_UNDERFLOW	0x00000008U
#define	FE_INEXACT	0x00000010U
#define	FE_DENORMAL	0x00000080U
#define	FE_ALL_EXCEPT	(FE_DIVBYZERO | FE_INEXACT | FE_DENORMAL | \
			 FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)
#else
#define	FE_INVALID	0x00000001
#define	FE_DIVBYZERO	0x00000002
#define	FE_OVERFLOW	0x00000004
#define	FE_UNDERFLOW	0x00000008
#define	FE_INEXACT	0x00000010
#define	FE_ALL_EXCEPT	(FE_DIVBYZERO | FE_INEXACT | \
			 FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)
#endif

/*
 * Rounding modes
 *
 * We can't just use the hardware bit values here, because that would
 * make FE_UPWARD and FE_DOWNWARD negative, which is not allowed.
 */
#ifdef __QNXNTO__
#define	FE_TONEAREST	0x0U
#define	FE_UPWARD	0x1U
#define	FE_DOWNWARD	0x2U
#define	FE_TOWARDZERO	0x3U
#define	_ROUND_MASK	(FE_TONEAREST | FE_DOWNWARD | \
			 FE_UPWARD | FE_TOWARDZERO)
#define	_ROUND_SHIFT	22U
#else
#define	FE_TONEAREST	0x0
#define	FE_UPWARD	0x1
#define	FE_DOWNWARD	0x2
#define	FE_TOWARDZERO	0x3
#define	_ROUND_MASK	(FE_TONEAREST | FE_DOWNWARD | \
			 FE_UPWARD | FE_TOWARDZERO)
#define	_ROUND_SHIFT	22
#endif

__BEGIN_DECLS

/* Default floating-point environment */
extern const fenv_t	__fe_dfl_env;
#define	FE_DFL_ENV	(&__fe_dfl_env)

/* We need to be able to map status flag positions to mask flag positions */
#ifdef __QNXNTO__
#define	_ENABLE_MASK	(FE_ALL_EXCEPT)
#else
#define _FPUSW_SHIFT	8
#define	_ENABLE_MASK	((_Uint64t)FE_ALL_EXCEPT << _FPUSW_SHIFT)
#endif

#define	__mrs_fpcr(__r)	({ __asm __volatile("mrs %0, fpcr" : "=r" (__r)); })
#define	__msr_fpcr(__r)	({ __asm __volatile("msr fpcr, %0" : : "r" (__r)); })

#define	__mrs_fpsr(__r)	({ __asm __volatile("mrs %0, fpsr" : "=r" (__r)); })
#define	__msr_fpsr(__r)	({ __asm __volatile("msr fpsr, %0" : : "r" (__r)); })

__fenv_static __inline__ int __attribute__((__unused__))
feclearexcept(const int __excepts)
{
	fexcept_t __r;

	__mrs_fpsr(__r);
	__r &= (fexcept_t)~__excepts;
	__msr_fpsr(__r);
	return (0);
}

__fenv_static __inline__ int __attribute__((__unused__))
fegetexceptflag(fexcept_t * const __flagp, const int __excepts)
{
	fexcept_t __r;

	__mrs_fpsr(__r);
	*__flagp = __r & (fexcept_t)__excepts;
	return (0);
}

__fenv_static __inline__ int __attribute__((__unused__))
fesetexceptflag(const fexcept_t * const __flagp, const int __excepts)
{
	fexcept_t __r;

	__mrs_fpsr(__r);
	__r &= (fexcept_t)~__excepts;
	__r |= *__flagp & (fexcept_t)__excepts;
	__msr_fpsr(__r);
	return (0);
}

__fenv_static __inline__ int __attribute__((__unused__))
feraiseexcept(const int __excepts)
{
	fexcept_t __r;

	__mrs_fpsr(__r);
	__r |= (fexcept_t)__excepts;
	__msr_fpsr(__r);
	return (0);
}

__fenv_static __inline__ int __attribute__((__unused__))
fetestexcept(const int __excepts)
{
	fexcept_t __r;

	__mrs_fpsr(__r);
	return (__r & (fexcept_t)__excepts);
}

__fenv_static __inline__ int __attribute__((__unused__))
fegetround(void)
{
#ifdef __QNXNTO__
	_Uint32t __r;

	__mrs_fpcr(__r);
	return ((__r >> _ROUND_SHIFT) & _ROUND_MASK);
#else
	fenv_t __r;

	__mrs_fpcr(__r);
	return ((__r >> _ROUND_SHIFT) & (fexcept_t)_ROUND_MASK);
#endif
}

__fenv_static __inline__ int __attribute__((__unused__))
fesetround(const int __round)
{
#ifdef __QNXNTO__
	_Uint32t __r;
#else
	fenv_t __r;
#endif

	if (__round & ~_ROUND_MASK) {
		return (-1);
	}
	__mrs_fpcr(__r);
#ifdef __QNXNTO__
	__r &= ~(_ROUND_MASK << _ROUND_SHIFT);
	__r |= (__round << _ROUND_SHIFT);
#else
	__r &= (fexcept_t)~(_ROUND_MASK << _ROUND_SHIFT);
	__r |= (fexcept_t)(__round << _ROUND_SHIFT);
#endif
	__msr_fpcr(__r);
	return (0);
}

__fenv_static __inline__ int __attribute__((__unused__))
fegetenv(fenv_t * const __envp)
{
#ifdef __QNXNTO__
	_Uint32t __r;
#else
	fenv_t __r;
#endif

	__mrs_fpcr(__r);
#ifdef __QNXNTO__
	__envp->__control = __r;
#else
	*__envp = __r & _ENABLE_MASK;
#endif

	__mrs_fpsr(__r);
#ifdef __QNXNTO__
	__envp->__status = __r;
#else
	*__envp |= __r & (fexcept_t)(FE_ALL_EXCEPT | (_ROUND_MASK << _ROUND_SHIFT));
#endif

	return (0);
}

__fenv_static __inline__ int __attribute__((__unused__))
feholdexcept(fenv_t * const __envp)
{
#ifdef __QNXNTO__
	_Uint32t __r;
#else
	fenv_t __r;
#endif

	__mrs_fpcr(__r);
#ifdef __QNXNTO__
	__envp->__control = __r;
#else
	*__envp = __r & _ENABLE_MASK;
	__r &= ~(_ENABLE_MASK);
	__msr_fpcr(__r);
#endif

	__mrs_fpsr(__r);
#ifdef __QNXNTO__
	__envp->__status = __r;
#else
	*__envp |= __r & (fexcept_t)(FE_ALL_EXCEPT | (_ROUND_MASK << _ROUND_SHIFT));
#endif

	__r &= ~(_ENABLE_MASK);
	__msr_fpsr(__r);
	return (0);
}

__fenv_static __inline__ int __attribute__((__unused__))
fesetenv(const fenv_t *__envp)
{

#ifdef __QNXNTO__
	__msr_fpcr(__envp->__control);
	__msr_fpsr(__envp->__status);
#else
	__msr_fpcr((*__envp) & _ENABLE_MASK);
	__msr_fpsr((*__envp) & (fexcept_t)(FE_ALL_EXCEPT | (_ROUND_MASK << _ROUND_SHIFT)));
#endif
	return (0);
}

__fenv_static __inline__ int __attribute__((__unused__))
feupdateenv(const fenv_t * const __envp)
{
	fexcept_t __r;

	__mrs_fpsr(__r);
	fesetenv(__envp);
	feraiseexcept((int)(__r & (fexcept_t)FE_ALL_EXCEPT));
	return (0);
}

#if defined(__EXT_BSD) || defined(__EXT_QNX)

/* We currently provide no external definitions of the functions below. */
/* AARCH64 does not support FPE masking */

static __inline__ int __attribute__((__unused__))
feenableexcept(const int __mask)
{
#ifdef __QNXNTO__
	/* no FPE can/were masked */
	return (0);
#else
	fenv_t __old_r, __new_r;

	__mrs_fpcr(__old_r);
	__new_r = __old_r | (fexcept_t)((__mask & FE_ALL_EXCEPT) << _FPUSW_SHIFT);
	__msr_fpcr(__new_r);
	return ((__old_r >> _FPUSW_SHIFT) & (fexcept_t)FE_ALL_EXCEPT);
#endif
}

static __inline__ int __attribute__((__unused__))
fedisableexcept(const int __mask)
{
#ifdef __QNXNTO__
	/* no FPE can/were masked */
	return (FE_ALL_EXCEPT);
#else
	fenv_t __old_r, __new_r;

	__mrs_fpcr(__old_r);
	__new_r = __old_r & (fexcept_t)~((__mask & FE_ALL_EXCEPT) << _FPUSW_SHIFT);
	__msr_fpcr(__new_r);
	return ((__old_r >> _FPUSW_SHIFT) & (fexcept_t)FE_ALL_EXCEPT);
#endif
}

static __inline__ int __attribute__((__unused__))
fegetexcept(void)
{
#ifdef __QNXNTO__
	/* no FPE can/were masked */
	return ((int)_ENABLE_MASK);
#else
	fenv_t __r;

	__mrs_fpcr(__r);
	return ((__r & _ENABLE_MASK) >> _FPUSW_SHIFT);
#endif
}

#endif /* defined(__EXT_BSD) || defined(__EXT_QNX) */

__END_DECLS

#endif	/* !_FENV_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/m/dist/freebsd/lib/msun/aarch64/fenv.h $ $Rev: 859618 $")
#endif
