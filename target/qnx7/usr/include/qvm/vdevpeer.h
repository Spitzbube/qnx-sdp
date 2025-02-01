/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems. All Rights Reserved.
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
 * API to communicate with a QVM guest's vdev.
 */

#ifndef __QVM_VDEVPEER_H
#define __QVM_VDEVPEER_H

#include <sys/types.h>
#include <sys/resmgr.h>

enum vdevpeer_transports {
    VDEVPEER_TRANSPORT_RAW      = 0,
    VDEVPEER_TRANSPORT_NET      = 1,
};

enum vdevpeer_link_state {
    VDEVPEER_LINK_UP,
    VDEVPEER_LINK_DOWN,
    VDEVPEER_LINK_RECONNECT
};

typedef struct qvm_vdevpeer_s qvm_vdevpeer_t;
typedef int (*qvm_vdevpeer_mgr_handler_t)(resmgr_context_t *ctp, io_msg_t *msg, void *arg);

int             qvm_vdevpeer_bind(qvm_vdevpeer_t *peer, const char *own_path);
int             qvm_vdevpeer_bind_mode(qvm_vdevpeer_t *peer, const char *own_path, unsigned mode);
int             qvm_vdevpeer_chid(const qvm_vdevpeer_t *peer);
void            qvm_vdevpeer_close(qvm_vdevpeer_t *peer);
int             qvm_vdevpeer_connect(qvm_vdevpeer_t *peer, const char *peer_path);
void            qvm_vdevpeer_destroy(qvm_vdevpeer_t *peer);
int             qvm_vdevpeer_devctl(qvm_vdevpeer_t *peer, resmgr_context_t *ctp, io_devctl_t *msg);
void            qvm_vdevpeer_flags(qvm_vdevpeer_t *peer, unsigned flags);
qvm_vdevpeer_t  *qvm_vdevpeer_init(const char *peer_path, unsigned flags);
int             qvm_vdevpeer_listen(qvm_vdevpeer_t *peer);
int             qvm_vdevpeer_pre_send(qvm_vdevpeer_t *peer, int *pcoid);
void            qvm_vdevpeer_post_send(qvm_vdevpeer_t *peer, int error);
ssize_t         qvm_vdevpeer_receivev(const qvm_vdevpeer_t *peer, const iov_t *riov, size_t rparts);
int             qvm_vdevpeer_register_iomgr(qvm_vdevpeer_t *peer, _Uint16t mgrid,
                                            qvm_vdevpeer_mgr_handler_t handler, void *arg);
void            qvm_vdevpeer_reset(qvm_vdevpeer_t *peer);
ssize_t         qvm_vdevpeer_sendv(qvm_vdevpeer_t *peer, const iov_t *siov, size_t sparts,
                                    const iov_t *riov, size_t rparts);
void            qvm_vdevpeer_set_transport(qvm_vdevpeer_t *peer, enum vdevpeer_transports transport);
int             qvm_vdevpeer_state(const qvm_vdevpeer_t *peer);

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/hyp/public/qvm/vdevpeer.h $ $Rev: 927509 $")
#endif
