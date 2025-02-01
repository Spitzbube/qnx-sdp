/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems. All Rights Reserved.
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
 * API to control a QVM guest's vdev.
 * Only interrupts for the time being, leaving the door open to a more complete
 * coverage of a vdev functionality;  i.e., registering remote callbacks for vread
 * and vwrite events, possibly timers.
 */

#ifndef __QVM_REMDEV_H
#define __QVM_REMDEV_H

#include <stdint.h>
#include <qvm/vdev.h>

enum {
    RDC_INTR = 0x00000001,  /**< Control a device's legacy interrupt line. */
    RDC_MSI  = 0x00000002,  /**< Send MSI frames on behalf of the device. */
};

enum {
    RDF_HWINTR = 0x00000001,    /**< Library can inject legacy interrupts while vCPU is running */
    RDF_HWMSI =  0x00000002,    /**< Library can inject MSI interrupts while vCPU is running */
};

typedef struct qvm_remdev_hdl_s qvm_remdev_hdl_t;

qvm_remdev_hdl_t     *qvm_remdev_attach(const char *device, unsigned controls);
int     qvm_remdev_detach(qvm_remdev_hdl_t *rdhdl);

int     qvm_remdev_signal_intr(qvm_remdev_hdl_t *rdhdl, enum qvm_intr_state state);
int     qvm_remdev_signal_msi(qvm_remdev_hdl_t *rdhdl, uint64_t addr, uint32_t data);

#define IOMGR_QVM_REMDEV    (_IOMGR_PRIVATE_BASE)

#define QVM_REMDEV_ATTACH   0
#define QVM_REMDEV_CPUDATA  1
#define QVM_REMDEV_DETACH   2
#define QVM_REMDEV_INTR     3
#define QVM_REMDEV_MSI      4

struct qvm_remdev_msg_attach {
    uint32_t controls;
    uint32_t reserved[3];
};

struct qvm_remdev_msg_attach_reply {
    uint32_t features;
    uint32_t num_cpus;
};
#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/hyp/public/qvm/remdev.h $ $Rev: 856346 $")
#endif
