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

/**
 * @file
 * Definitions for VirtIO Networking over vdevpeer.
 */

#ifndef __QVM_VDEVPEER_NET_H
#define __QVM_VDEVPEER_NET_H

#include <sys/platform.h>

struct vdevpeer_net_hdr {
    _Uint32t    vh_version;
    _Uint16t    vh_num_packets;
    _Uint16t    vh_features;
    _Uint32t    vh_lengths[];
};

#define VDEVPEER_NET_VERSION    20190528

#define VN_FEAT_NUM_BUFFERS     (1u << 0)
#define VN_FEAT_TSO4            (1u << 1)
#define VN_FEAT_TSO6            (1u << 2)
#define VN_FEAT_UFO             (1u << 3)
#define VN_FEAT_ECN             (1u << 4)
#define VN_FEAT_CSUM            (1u << 5)

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/hyp/public/qvm/vdevpeer-net.h $ $Rev: 895206 $")
#endif
