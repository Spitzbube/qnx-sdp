/*
 * $QNXtpLicenseC:
 * Copyright 2010-2020, QNX Software Systems. All Rights Reserved.
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

#ifndef _STRING_H_INCLUDED
#error <string_chk.h> should not be included directly.
#endif

#ifndef _STRING_CHK_H_INCLUDED
#define _STRING_CHK_H_INCLUDED

#include <fortify.h>


__BEGIN_DECLS


__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_memcpy,
        __fortify_fail_overflow_dst,
        "argument 3 of 'memcpy' is greater than the length of the object referenced by argument 1");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag___memcpy_isr,
        __fortify_fail_overflow_dst,
        "argument 3 of '__memcpy_isr' is greater than the length of the object referenced by argument 1");
extern void *__ALIAS(__memcpy_isr_alias, (void *__s1, const void *__s2, size_t __n), "__memcpy_isr");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_memmove,
        __fortify_fail_overflow_dst,
        "argument 3 of 'memmove' is greater than the length of the object referenced by argument 1");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_memset,
        __fortify_fail_overflow_dst,
        "argument 3 of 'memset' is greater than the length of the object referenced by argument 1");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag___memset_isr,
        __fortify_fail_overflow_dst,
        "argument 3 of '__memset_isr' is greater than the length of the object referenced by argument 1");
extern void *__ALIAS(__memset_isr_alias, (void *__s, int __c, size_t __n), "__memset_isr");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_stpcpy,
        __fortify_fail_overflow_dst,
        "argument 2 of 'stpcpy' references a string longer than the object referenced by argument 1");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_stpncpy,
        __fortify_fail_overflow_dst,
        "argument 3 of 'stpncpy' is greater than the length of the object referenced by argument 1");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_strcat,
        __fortify_fail_overflow_dst,
        "argument 2 of 'strcat' references a string longer than the object referenced by argument 1");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_strcpy,
        __fortify_fail_overflow_dst,
        "argument 2 of 'strcpy' references a string longer than the object referenced by argument 1");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag___strcpy_isr,
        __fortify_fail_overflow_dst,
        "argument 2 of '__strcpy_isr' references a string longer than the object referenced by argument 1");
extern char *__ALIAS(__strcpy_isr_alias, (char *__s1, const char *__s2), "__strcpy_isr");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_strlcat,
        __fortify_fail_overflow_dst,
        "argument 3 of 'strlcat' is greater than the length of the object referenced by argument 1");
extern size_t __ALIAS(__strlcat_alias, (char *__s1, const char *__s2, size_t __n), "strlcat");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_strlcpy,
        __fortify_fail_overflow_dst,
        "argument 3 of 'strlcpy' is greater than the length of the object referenced by argument 1");
extern size_t __ALIAS(__strlcpy_alias, (char *__s1, const char *__s2, size_t __n), "strlcpy");

extern char *__strncat_chk(char *__s1, const char *__s2, size_t __n, size_t __osize);
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_strncat,
        __fortify_fail_overflow_dst,
        "argument 3 of 'strncat' is greater than or equal to the length of the object referenced by argument 1");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_strncpy,
        __fortify_fail_overflow_dst,
        "argument 3 of 'strncpy' is greater than the length of the object referenced by argument 1");

#if defined(__EXT_POSIX1_200112) || defined(__EXT_XOPEN_EX)
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_memccpy,
        __fortify_fail_overflow_dst,
        "argument 4 of 'memccpy' is greater than the length of the object referenced by argument 1");
extern void *__ALIAS(__memccpy_alias, (void * __restrict __s1, const void * __restrict __s2, int __c, size_t __n), "memccpy");
#endif

#if defined(__EXT_QNX)
__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_memcpyv,
        __fortify_fail_overflow_dst,
        "argument 2 of 'memcpyv' is negative or greater than the length (in 'struct iovec' elements) of the object referenced by "
                "argument 1");
extern size_t __ALIAS(
        __memcpyv_alias,
        (const struct iovec *__dst, int __dparts, size_t __doff, const struct iovec *__src, int __sparts, size_t __soff),
        "memcpyv");

__FORTIFY_FAIL_ALIAS_DECL(
        __fortify_fail_overflow_dst_diag_memset_s,
        __fortify_fail_overflow_dst,
        "argument 2 of 'memset_s' is greater than the length of the object referenced by argument 1");
extern errno_t __ALIAS(__memset_s_alias, (void *__v, rsize_t __smax, int __c, rsize_t __n), "memset_s");
#endif


extern __FORTIFY_FUNCTION void *memcpy(void *__dst, const void *__src, size_t __nbytes)
{
    /* Fail if the number of bytes to be copied (__nbytes) is greater than the length of the destination object (__dst) determined
     * at compile time. Also emit an error/warning if the __nbytes argument is known to be constant at compile time. __bos0 is used
     * to obtain the length of the destination object instead of __bos in order to reduce false positives when the _FORTIFY_SOURCE
     * feature test macro has been defined with a value greater than 1. */
    if ( __bos0(__dst) != (size_t)-1 ) {
        if ( __bconst(__nbytes) ) {
            if ( __unlikely(__nbytes > __bos0(__dst)) ) {
                __fortify_fail_overflow_dst_diag_memcpy();
            }
        } else {
            if ( __unlikely(__nbytes > __bos0(__dst)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    /* Leverage the compiler's built-in memcpy function, which can potentially be optimized into something more performant than a
     * call to the regular (unfortified) memcpy function */
    return __builtin_memcpy(__dst, __src, __nbytes);
}

extern __FORTIFY_FUNCTION void *__memcpy_isr(void *__s1, const void *__s2, size_t __n)
{
    /* Fail if the number of bytes to be copied (__n) is greater than the length of the destination object (__s1) determined at
     * compile time. Also emit an error/warning if the __n argument is known to be constant at compile time. __bos0 is used to
     * obtain the length of the destination object instead of __bos in order to reduce false positives when the _FORTIFY_SOURCE
     * feature test macro has been defined with a value greater than 1. */
    if ( __bos0(__s1) != (size_t)-1 ) {
        if ( __bconst(__n) ) {
            if ( __unlikely(__n > __bos0(__s1)) ) {
                __fortify_fail_overflow_dst_diag___memcpy_isr();
            }
        } else {
            if ( __unlikely(__n > __bos0(__s1)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    return __memcpy_isr_alias(__s1, __s2, __n);
}

extern __FORTIFY_FUNCTION void *memmove(void *__s1, const void *__s2, size_t __n)
{
    /* Fail if the number of bytes to be copied (__n) is greater than the length of the destination object (__s1) determined at
     * compile time. Also emit an error/warning if the __n argument is known to be constant at compile time. __bos0 is used to
     * obtain the length of the destination object instead of __bos in order to reduce false positives when the _FORTIFY_SOURCE
     * feature test macro has been defined with a value greater than 1. */
    if ( __bos0(__s1) != (size_t)-1 ) {
        if ( __bconst(__n) ) {
            if ( __unlikely(__n > __bos0(__s1)) ) {
                __fortify_fail_overflow_dst_diag_memmove();
            }
        } else {
            if ( __unlikely(__n > __bos0(__s1)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    /* Leverage the compiler's built-in memmove function, which can potentially be optimized into something more performant than a
     * call to the regular (unfortified) memmove function */
    return __builtin_memmove(__s1, __s2, __n);
}

extern __FORTIFY_FUNCTION void *memset(void *__s, int __c, size_t __n)
{
    /* Fail if the number of bytes to be set (__n) is greater than the length of the destination object (__s) determined at compile
     * time. Also emit an error/warning if the __n argument is known to be constant at compile time. __bos0 is used to obtain the
     * length of the destination object instead of __bos in order to reduce false positives when the _FORTIFY_SOURCE feature test
     * macro has been defined with a value greater than 1. */
    if ( __bos0(__s) != (size_t)-1 ) {
        if ( __bconst(__n) ) {
            if ( __unlikely(__n > __bos0(__s)) ) {
                __fortify_fail_overflow_dst_diag_memset();
            }
        } else {
            if ( __unlikely(__n > __bos0(__s)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    /* Leverage the compiler's built-in memset function, which can potentially be optimized into something more performant than a
     * call to the regular (unfortified) memset function */
    return __builtin_memset(__s, __c, __n);
}

extern __FORTIFY_FUNCTION void *__memset_isr(void *__s, int __c, size_t __n)
{
    /* Fail if the number of bytes to be copied (__n) is greater than the length of the destination object (__s) determined at
     * compile time. Also emit an error/warning if the __n argument is known to be constant at compile time. __bos0 is used to
     * obtain the length of the destination object instead of __bos in order to reduce false positives when the _FORTIFY_SOURCE
     * feature test macro has been defined with a value greater than 1. */
    if ( __bos0(__s) != (size_t)-1 ) {
        if ( __bconst(__n) ) {
            if ( __unlikely(__n > __bos0(__s)) ) {
                __fortify_fail_overflow_dst_diag___memset_isr();
            }
        } else {
            if ( __unlikely(__n > __bos0(__s)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    return __memset_isr_alias(__s, __c, __n);
}

extern __FORTIFY_FUNCTION char *stpcpy(char * _Restrict __s1, const char * _Restrict __s2)
{
    /* Fail and emit an error/warning if the source string (__s2) including the NUL terminator is known at compile time to be
     * longer than the destination object (__s1) */
    if ( ( __bos(__s1) != (size_t)-1 ) && __bconst(__builtin_strlen(__s2)) && ( __builtin_strlen(__s2) >= __bos(__s1) ) ) {
        __fortify_fail_overflow_dst_diag_stpcpy();
    }
    /* Leverage the compiler's built-in __stpcpy_chk function which resolves to a call to the __stpcpy_chk function or a compatible
     * but more performant alternative when the third argument is a value other than (size_t)-1 (i.e. the length of the destination
     * object (__s1) is known at compile time). Otherwise, it resolves to a call to the regular (unfortified) stpcpy function or a
     * compatible but more performant alternative. __stpcpy_chk calls __fortify_fail_overflow_dst if the length of the source string
     * (__s2) plus a NUL terminator is greater than the length of the destination object (__s1) determined at compile time. */
    return __builtin___stpcpy_chk(__s1, __s2, __bos(__s1));
}

extern __FORTIFY_FUNCTION char *stpncpy(char * _Restrict __s1, const char * _Restrict __s2, size_t __n)
{
    /* Fail if the maximum number of characters to be copied (__n) is greater than the length of the destination object (__s1)
     * determined at compile time. Also emit an error/warning if the __n argument is known to be constant at compile time. */
    if ( __bos(__s1) != (size_t)-1 ) {
        if ( __bconst(__n) ) {
            if ( __unlikely(__n > __bos(__s1)) ) {
                __fortify_fail_overflow_dst_diag_stpncpy();
            }
        } else {
            if ( __unlikely(__n > __bos(__s1)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    /* Leverage the compiler's built-in stpncpy function, which can potentially be optimized into something more performant than a
     * call to the regular (unfortified) stpncpy function */
    return __builtin_stpncpy(__s1, __s2, __n);
}

extern __FORTIFY_FUNCTION char *strcat(char *__s1, const char *__s2)
{
    /* Fail and emit an error/warning if the source string (__s2) including the NUL terminator is known at compile time to be
     * longer than the destination object (__s1) */
    if ( ( __bos(__s1) != (size_t)-1 ) && __bconst(__builtin_strlen(__s2)) && ( __builtin_strlen(__s2) >= __bos(__s1) ) ) {
        __fortify_fail_overflow_dst_diag_strcat();
    }
    /* Leverage the compiler's built-in __strcat_chk function which resolves to a call to the __strcat_chk function or a compatible
     * but more performant alternative when the third argument is a value other than (size_t)-1 (i.e. the length of the destination
     * object (__s1) is known at compile time). Otherwise, it resolves to a call to the regular (unfortified) strcat function or a
     * compatible but more performant alternative. __strcat_chk calls __fortify_fail_overflow_dst if the length of the concatenated
     * string plus the terminating NUL character is greater than the length of the destination object (__s1) determined at compile
     * time. */
    return __builtin___strcat_chk(__s1, __s2, __bos(__s1));
}

extern __FORTIFY_FUNCTION char *strcpy(char *__s1, const char *__s2)
{
    /* Fail if the length of the source string (__s2) including the terminating NUL character is greater than the length of the
     * destination object (__s1) determined at compile time. Also emit an error/warning if the length of the source string (__s2) is
     * known to be constant at compile time. */
    if ( __bos(__s1) != (size_t)-1 ) {

        /* Leverage the compiler's built-in strlen function, which can potentially be optimized into something more performant than
         * a call to the strlen function */
        const size_t __len = __builtin_strlen(__s2);

        if ( __bconst(__len) ) {
            if ( __unlikely(__len >= __bos(__s1)) ) {
                __fortify_fail_overflow_dst_diag_strcpy();
            }
        } else {
            if ( __unlikely(__len >= __bos(__s1)) ) {
                __fortify_fail_overflow_dst();
            }
        }

        /* Since we already know the length of the source string, use memcpy rather than strcpy to perform the copy operation.
         * Leverage the compiler's built-in memcpy function, which can potentially be optimized into something more performant than
         * a call to the regular (unfortified) memcpy function. */
        return (char *)__builtin_memcpy(__s1, __s2, __len + 1);

    /* Otherwise, leverage the compiler's built-in strcpy function, which can potentially be optimized into something more
     * performant than a call to the regular (unfortified) strcpy function */
    } else {
        return __builtin_strcpy(__s1, __s2);
    }
}

extern __FORTIFY_FUNCTION char *__strcpy_isr(char *__s1, const char *__s2)
{
    /* Fail if the length of the source string (__s2) including the terminating NUL character is greater than the length of the
     * destination object (__s1) determined at compile time. Also emit an error/warning if the length of the source string (__s2) is
     * known to be constant at compile time. */
    if ( __bos(__s1) != (size_t)-1 ) {

        size_t __len;

        if ( __bconst(__builtin_strlen(__s2)) ) {
            __len = __builtin_strlen(__s2);
            if ( __len >= __bos(__s1) ) {
                __fortify_fail_overflow_dst_diag___strcpy_isr();
            }
        } else {
            __len = __strlen_isr(__s2);
            if ( __unlikely(__len >= __bos(__s1)) ) {
                __fortify_fail_overflow_dst();
            }
        }

        /* Since we already know the length of the source string, use __memcpy_isr rather than __strcpy_isr to perform the copy
         * operation */
        return (char *)__memcpy_isr_alias(__s1, __s2, __len + 1);

    } else {
        return __strcpy_isr_alias(__s1, __s2);
    }
}

extern __FORTIFY_FUNCTION size_t strlcat(char *__s1, const char *__s2, size_t __n)
{
    /* Fail if the maximum string length including the terminating NUL character (__n) is greater than the length of the destination
     * object (__s1) determined at compile time. Also emit an error/warning if the __n argument is known to be constant at compile
     * time. */
    if ( __bos(__s1) != (size_t)-1 ) {
        if ( __bconst(__n) ) {
            if ( __unlikely(__n > __bos(__s1)) ) {
                __fortify_fail_overflow_dst_diag_strlcat();
            }
        } else {
            if ( __unlikely(__n > __bos(__s1)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    return __strlcat_alias(__s1, __s2, __n);
}

extern __FORTIFY_FUNCTION size_t strlcpy(char *__s1, const char *__s2, size_t __n)
{
    /* Fail if the maximum number of characters to copy including the terminating NUL character (__n) is greater than the length of
     * the destination object (__s1) determined at compile time. Also emit an error/warning if the __n argument is known to be
     * constant at compile time. */
    if ( __bos(__s1) != (size_t)-1 ) {
        if ( __bconst(__n) ) {
            if ( __unlikely(__n > __bos(__s1)) ) {
                __fortify_fail_overflow_dst_diag_strlcpy();
            }
        } else {
            if ( __unlikely(__n > __bos(__s1)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    return __strlcpy_alias(__s1, __s2, __n);
}

extern __FORTIFY_FUNCTION char *strncat(char *__s1, const char *__s2, size_t __n)
{
    /* Fail and emit an error/warning if the maximum number of characters to append including the terminating NUL character
     * (__n + 1) is greater than the length of the destination object (__s1) determined at compile time */
    if ( ( __bos(__s1) != (size_t)-1 ) && __bconst(__n) && ( __n >= __bos(__s1) ) ) {
        __fortify_fail_overflow_dst_diag_strncat();
    }
    /* Leverage the compiler's built-in __strncat_chk function which resolves to a call to the __strncat_chk function or a
     * compatible but more performant alternative when the third argument is a value other than (size_t)-1 (i.e. the length of
     * the destination object (__s1) is known at compile time). Otherwise, it resolves to a call to the regular (unfortified)
     * strncat function or a compatible but more performant alternative. __strncat_chk calls __fortify_fail_overflow_dst if the
     * length of the concatenated string plus the terminating NUL character is greater than the length of the destination
     * object (__s1) determined at compile time. */
    return __builtin___strncat_chk(__s1, __s2, __n, __bos(__s1));
}

extern __FORTIFY_FUNCTION char *strncpy(char *__s1, const char *__s2, size_t __n)
{
    /* Fail if the maximum number of characters to copy (__n) is greater than the length of the destination object (__s1) determined
     * at compile time. Also emit an error/warning if the __n argument is known to be constant at compile time. */
    if ( __bos(__s1) != (size_t)-1 ) {
        if ( __bconst(__n) ) {
            if ( __unlikely(__n > __bos(__s1)) ) {
                __fortify_fail_overflow_dst_diag_strncpy();
            }
        } else {
            if ( __unlikely(__n > __bos(__s1)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    /* Leverage the compiler's built-in strncpy function, which can potentially be optimized into something more performant than a
     * call to the regular (unfortified) strncpy function */
    return __builtin_strncpy(__s1, __s2, __n);
}

#if defined(__EXT_POSIX1_200112) || defined(__EXT_XOPEN_EX)
extern __FORTIFY_FUNCTION void *memccpy(void * __restrict __s1, const void * __restrict __s2, int __c, size_t __n)
{
    /* Fail if the maximum number of bytes to be copied (__n) is greater than the length of the destination object (__s1) determined
     * at compile time. Also emit an error/warning if the __n argument is known to be constant at compile time. __bos0 is used to
     * obtain the length of the destination object instead of __bos in order to reduce false positives when the _FORTIFY_SOURCE
     * feature test macro has been defined with a value greater than 1. */
    if ( __bos0(__s1) != (size_t)-1 ) {
        if ( __bconst(__n) ) {
            if ( __unlikely(__n > __bos0(__s1)) ) {
                __fortify_fail_overflow_dst_diag_memccpy();
            }
        } else {
            if ( __unlikely(__n > __bos0(__s1)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    return __memccpy_alias(__s1, __s2, __c, __n);
}
#endif

#if defined(__EXT_QNX)
extern __FORTIFY_FUNCTION size_t memcpyv(
        const struct iovec *__dst,
        int                 __dparts,
        size_t              __doff,
        const struct iovec *__src,
        int                 __sparts,
        size_t              __soff)
{
    /* Fail if the specified number of struct iovec objects (__dparts) is greater than the number of elements in the destination
     * struct iovec array (__dst) determined at compile time. Also emit an error/warning if the __dparts argument is known to be
     * constant at compile time. */
    if ( __bos(__dst) != (size_t)-1 ) {
        if ( __bconst(__dparts) ) {
            if ( __unlikely((unsigned int)__dparts > (__bos(__dst) / sizeof __dst[0])) ) {
                __fortify_fail_overflow_dst_diag_memcpyv();
            }
        } else {
            if ( __unlikely((unsigned int)__dparts > (__bos(__dst) / sizeof __dst[0])) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    return __memcpyv_alias(__dst, __dparts, __doff, __src, __sparts, __soff);
}

extern __FORTIFY_FUNCTION errno_t memset_s(void *__v, rsize_t __smax, int __c, rsize_t __n)
{
    /* Fail if the maximum number of bytes to be set (__smax) is greater than the length of the destination object (__v) determined
     * at compile time. Also emit an error/warning if the __smax argument is known to be constant at compile time. __bos0 is used to
     * obtain the length of the destination object instead of __bos in order to reduce false positives when the _FORTIFY_SOURCE
     * feature test macro has been defined with a value greater than 1. */
    if ( __bos0(__v) != (size_t)-1 ) {
        if ( __bconst(__smax) ) {
            if ( __unlikely(__smax > __bos0(__v)) ) {
                __fortify_fail_overflow_dst_diag_memset_s();
            }
        } else {
            if ( __unlikely(__smax > __bos0(__v)) ) {
                __fortify_fail_overflow_dst();
            }
        }
    }
    return __memset_s_alias(__v, __smax, __c, __n);
}
#endif


__END_DECLS

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/c/public/string_chk.h $ $Rev: 916325 $")
#endif
