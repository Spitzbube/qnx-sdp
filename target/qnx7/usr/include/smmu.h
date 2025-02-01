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
 * System memory management unit interface definitions
 */

#ifndef _SMMU_H_INCLUDED
#define _SMMU_H_INCLUDED

#include <sys/platform.h>

#if defined(__PID_T)
typedef __PID_T		pid_t;
#undef __PID_T
#endif

#define SMMU_ABILITY_ATTACH_NAME	"smmu/attach"
#define SMMU_ABILITY_TARGET_NAME	"smmu/target"

struct sigevent;


/** Type of device id */
enum smmu_devid_type {
	SDT_PCI,
	SDT_MMIO,
	SDT_NUM_TYPES,
};

/** PCI device id */
struct smmu_devid_pci {
	unsigned	type;
	unsigned	bus;
	unsigned	dev;
	unsigned	func;
};

/** Allows any value for a PCI bus, dev or func number */
#define SMMU_PCI_FIELD_ANY	(~0u)

/** MMIO device id */
struct smmu_devid_mmio {
	unsigned	type;
	unsigned	length;
	_Uint64t	phys;
};


/** Generic device id */
union smmu_devid {
	unsigned				type;
	struct smmu_devid_pci	pci;
	struct smmu_devid_mmio	mmio;
	_Uint64t				__spacer[4];
};

/** SMMU mapping flag bits */
enum smmu_mapping_flags {
	SMF_NONE	= 0x00,
	SMF_READ	= 0x01,
	SMF_WRITE	= 0x02,
	SMF_EXEC	= 0x04,
	SMF_VIRT	= 0x08,
	SMF_TARGET	= 0x10,
};

struct smmu_map_entry {
	union {
		void		*virt;
		_Uint32t	virt32;
		_Uint64t	virt64;
		_Uint64t	phys;
	};
	_Uint64t	len;
};


struct smmu_status {
	_Uint64t			hw_specific;
	_Uint64t			fault_addr;
	union smmu_devid	devid;
	unsigned			flags;
};

#define SSF_DROPPED_FAULTS	0x80000000u

#define SMMUMAN_SAFETY_ERROR	(-1)
#define SMMUMAN_SAFETY_NO		0
#define SMMUMAN_SAFETY_YES		0xffff

int			smmu_init(unsigned __flags);
int			smmu_safety(void);
void		smmu_fini(void);
int			smmu_xfer_notify(const struct sigevent *__evp);
int 		smmu_xfer_status(struct smmu_status *__ssp);


enum smmu_obj_create_flags {
	SOCF_NONE 				= 0x0000,
	SOCF_RESERVED_MANUAL	= 0x0001,
};

struct smmu_object *smmu_obj_create(unsigned __flags);
void				smmu_obj_destroy(struct smmu_object *__sop);

int smmu_device_add_generic(struct smmu_object *__sop, const union smmu_devid *__devid);

static inline int
smmu_device_add_pci(struct smmu_object *const sop, unsigned const pbus, unsigned const pdev, unsigned const pfunc) {
	union smmu_devid	devid;
	devid.type = SDT_PCI;
	devid.pci.bus = pbus;
	devid.pci.dev = pdev;
	devid.pci.func = pfunc;
	return smmu_device_add_generic(sop, &devid);
}

static inline int
smmu_device_add_mmio(struct smmu_object *const sop, _Uint64t const phys, unsigned const len) {
	union smmu_devid	devid;
	devid.type = SDT_MMIO;
	devid.mmio.phys = phys;
	devid.mmio.length = len;
	return smmu_device_add_generic(sop, &devid);
}


unsigned smmu_device_report_reserved(const union smmu_devid *__devid, unsigned __offset, struct smmu_map_entry *__resv, unsigned __nresv);


int smmu_mapping_add(struct smmu_object *__sop,
			unsigned __flags, pid_t __pid, unsigned __num_entries,
			const struct smmu_map_entry *__entries, _Uint64t __target);

#endif


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/smmu/public/smmu.h $ $Rev: 876230 $")
#endif
