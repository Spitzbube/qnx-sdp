/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems. All Rights Reserved.
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
 * Host side QNX hypervisor interface definitions
 */

#ifndef __HYP_SHM_H
#define __HYP_SHM_H

/** Maximum number of clients that can connect to a shared memory region */
#define HYP_SHM_MAX_CLIENTS		16
/** Poke all the clients including myself */
#define HYP_SHM_POKE_ALL		((1u << HYP_SHM_MAX_CLIENTS) - 1)
/** Poke all the clients except for myself */
#define HYP_SHM_POKE_ALL_BUT_ME	(~0u)

struct hyp_shm;

extern struct hyp_shm *hyp_shm_create(unsigned __client_size);

extern int hyp_shm_attach(struct hyp_shm *__hsp, const char *__name, unsigned __shsize, int __chid, int __prio, int __code, const void *__value);

extern unsigned hyp_shm_size(const struct hyp_shm *__hsp);

extern void *hyp_shm_client(const struct hyp_shm *__hsp);

extern void *hyp_shm_data(const struct hyp_shm *__hsp);

extern const char *hyp_shm_name(const struct hyp_shm *__hsp);

extern unsigned hyp_shm_idx(const struct hyp_shm *__hsp);

extern int hyp_shm_poke(struct hyp_shm *__hsp, unsigned __target_list);

extern unsigned hyp_shm_status(struct hyp_shm *__hsp);

extern unsigned hyp_shm_verify(struct hyp_shm *__hsp, unsigned __target_list);

extern void hyp_shm_detach(struct hyp_shm *__hsp);


#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/hyp/public/hyp_shm.h $ $Rev: 878977 $")
#endif
