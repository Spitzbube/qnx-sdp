/*
 * $QNXLicenseC:
 * Copyright (c) 2018, QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software. Free development
 * licenses are available for evaluation and non-commercial purposes. For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others. Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#if defined(__X86__)
# include <openssl/x86/opensslconf.h>
#elif defined(__ARM__)
# include <openssl/armle-v7/opensslconf.h>
#elif defined(__X86_64__)
# include <openssl/x86_64/opensslconf.h>
#elif defined(__aarch64__)
# include <openssl/aarch64le/opensslconf.h>
#else
# error unsupported platform
#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
#ifdef __ASM__
__SRCVERSION "$URL: http://f27svn.qnx.com/svn/repos/osr/branches/7.0.0/trunk/openssl/build/public/opensslconf.h $ $Rev: 940 $"
#else
__SRCVERSION( "$URL: http://f27svn.qnx.com/svn/repos/osr/branches/7.0.0/trunk/openssl/build/public/opensslconf.h $ $Rev: 940 $" )
#endif
#endif