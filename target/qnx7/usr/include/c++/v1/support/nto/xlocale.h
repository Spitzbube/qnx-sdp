// -*- C++ -*-
//===------------------------ support/nto/xlocale.h -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
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
 * for other information
 * $
 */

#ifndef _LIBCPP_SUPPORT_NTO_XLOCALE_H
#define _LIBCPP_SUPPORT_NTO_XLOCALE_H

#if defined(__QNXNTO__)

#include <ctype.h>
#include <wctype.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef const char *locale_t;
#ifdef __cplusplus
}
#endif

#include <support/xlocale/__posix_l_fallback.h>
#include <support/xlocale/__strtonum_fallback.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int vasprintf(char **__strp, const char *__fmt, va_list __arg);

extern int isascii(int);

#define LC_COLLATE_MASK _M_COLLATE
#define LC_CTYPE_MASK _M_CTYPE
#define LC_MONETARY_MASK _M_MONETARY
#define LC_NUMERIC_MASK _M_NUMERIC
#define LC_TIME_MASK _M_TIME
#define LC_MESSAGES_MASK _M_MESSAGES
#define LC_ALL_MASK _M_ALL

static inline locale_t newlocale(int, const char *name, locale_t) {
    const char *const oldloc = setlocale(LC_ALL, NULL);
    const char *const newloc = setlocale(LC_ALL, name);
    setlocale(LC_ALL, oldloc);
    return newloc;
}

static inline locale_t uselocale(locale_t loc) {
    const char *const oldloc = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, loc);
    return oldloc;
}

static inline int freelocale(locale_t loc) {
  return 0;
}

static inline size_t mbsnrtowcs(wchar_t *dst, const char **src, size_t nmc, size_t nwc, mbstate_t *ps) {
  size_t i;
  const char *s = *src;

  if (!dst) {
    nwc = (size_t)-1;
  }

  for (i=0; i < nwc && nmc != 0; ++i) {
    wchar_t wc;
    const int len = mbrtowc(&wc, s, nmc, ps);
    if (len < 0) {
      return -1;
    }

    s += len;
    nmc -= len;
    if (dst) {
      dst[i] = wc;
      *src = s;
    }
    if (len == 0) {
      if (dst) {
        *src = NULL;
      }
      break;
    }
  }
  return i;
}

static inline size_t wcsnrtombs(char *dst, const wchar_t **src, size_t nwc, size_t nmc, mbstate_t *ps) {
  size_t nbytes = 0;
  size_t i;
  const wchar_t *const ws = *src;

  if (!dst) {
    nmc = (size_t)-1;
  }

  for (i=0; i < nwc && nmc > nbytes; ++i) {
    char buf[MB_LEN_MAX];
    const int len = wcrtomb(buf, ws[i], ps);
    if (len == -1) {
        return -1;
    }
    if ((nbytes+=len) > nmc) {
        errno = EILSEQ;
        return -1;
    }
    if (dst) {
      memcpy(dst, buf, len);
      dst += len;
      *src = &ws[i+1];
    }
    if (ws[i] == L'\0') {
      if (dst) {
        *src = NULL;
      }
      nbytes--;
      break;
    }
  }

  return nbytes;
}

#ifdef __cplusplus
}
#endif

#endif // defined(__QNXNTO__)
#endif // _LIBCPP_SUPPORT_NTO_XLOCALE_H

