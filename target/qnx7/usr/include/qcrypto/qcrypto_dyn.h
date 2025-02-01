/*
 * $QNXLicenseC:
 * Copyright (c) 2017-2019 QNX Software Systems. All Rights Reserved.
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

/**
 * @file qcrypto_dyn.h
 * @brief QNX cryptography library dynamic API
 *
 * This shim library is designed to allow client code to compile @c qcrypto library
 * support without explicitly linking in the library itself.
 * Instead, programs only need to link to @c libqcrypto_dyn.so.
 *
 * When @e qcrypto_load() is called, the library is opened via @e dlopen() and provides
 * an interface identical to the regular @c qcrypto library API.
 */

#ifndef _QCRYPTO_QCRYPTO_DYN_H_INCLUDED
#define _QCRYPTO_QCRYPTO_DYN_H_INCLUDED

#include <sys/cdefs.h>

#include <qcrypto/qcrypto.h>
#include <qcrypto/qcrypto_keys.h>

/** @cond */
__BEGIN_DECLS
/** @endcond */

/** @cond */
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif
/** @endcond */

/**
 * @brief Dynamically load the @c qcrypto library
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_load(void);

/**
 * @brief Unload the @c qcrypto library
 * @details All objects and handles from the library are no longer valid
 *          after this call.
 */
void qcrypto_unload(void);

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QCRYPTO_QCRYPTO_DYN_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qcrypto/qcrypto_dyn/public/qcrypto/qcrypto_dyn.h $ $Rev: 898062 $")
#endif
