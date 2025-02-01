/*
 * $QNXLicenseC:
 * Copyright 2017, 2019, QNX Software Systems. All Rights Reserved.
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

#ifndef _SYS_QVM_DCMD_H_
#define _SYS_QVM_DCMD_H_

#include <sys/platform.h>
#include <devctl.h>

/**
 * @file
 * Define devctl() commands recognized by the process container.
 */

#define DCMD_QVM_VDEVPEER_CONNECT   (__DIOTF(_DCMD_MISC, 0, int32_t))

struct vdevpeer_connect {
    _Int32t     chid;
    _Int32t     transport;
};

#define DCMD_QVM_VDEVPEER_CONNECT2  (__DIOTF(_DCMD_MISC, 0, struct vdevpeer_connect))

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/hyp/public/sys/dcmd_qvm.h $ $Rev: 895206 $")
#endif
