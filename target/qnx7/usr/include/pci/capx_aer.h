#ifndef _CAPX_AER_H_
#define _CAPX_AER_H_
/*
 * $QNXLicenseC:
 * Copyright (c) 2012, 2019 QNX Software Systems. All Rights Reserved.
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

#include <stdint.h>
#include <inttypes.h>

#include <pci/pci.h>
#include <pci/cap_pcie.h>

/*
 ===============================================================================

 This file contains public interfaces (functions and types) for accessing PCIe
 Device Extended Advanced Error Reporting specific capability data.

 The API's are only available after a successful call to cap_pcie_read_xtnd_cap()

 ===============================================================================
*/
#define CAPID_PCIe_AER		0x1

/*
 ===============================================================================
 Advanced Error Reporting PCIe extended capability specific types
*/
typedef uint32_t	capx_aer_status_t;
typedef uint32_t	capx_aer_mask_t;
typedef uint32_t	capx_aer_sev_t;
typedef uint16_t	capx_aer_requester_id_t;
typedef struct
{
	uint32_t	data[4];
} capx_aer_log_t;

/* bit masks for Uncorrectable status, mask and severity registers */
#define	CAPX_AER_UC_ERR_POISONED_TLP_EGRESS_BLOCKED		(1u << 26)
#define	CAPX_AER_UC_ERR_TLP_PREFIX_BLOCKED				(1u << 25)
#define	CAPX_AER_UC_ERR_ATOMIC_OP_EGRESS_BLOCKED		(1u << 24)
#define	CAPX_AER_UC_ERR_MC_BLOCKED_TLP					(1u << 23)
#define	CAPX_AER_UC_ERR_INTERNAL						(1u << 22)
#define	CAPX_AER_UC_ERR_ACS_VIOLATION					(1u << 21)
#define	CAPX_AER_UC_ERR_UNSUPPORTED_REQUEST				(1u << 20)
#define	CAPX_AER_UC_ERR_ECRC							(1u << 19)
#define	CAPX_AER_UC_ERR_MALFORMED_TLP					(1u << 18)
#define	CAPX_AER_UC_ERR_RECEIVER_OVERFLOW				(1u << 17)
#define	CAPX_AER_UC_ERR_UNEXPECTED_COMPLETION			(1u << 16)
#define	CAPX_AER_UC_ERR_COMPLETER_ABORT					(1u << 15)
#define	CAPX_AER_UC_ERR_COMPLETION_TIMEOUT				(1u << 14)
#define	CAPX_AER_UC_ERR_FLOW_CONTROL_PROTOCOL			(1u << 13)
#define	CAPX_AER_UC_ERR_POISONED_TLP					(1u << 12)
#define	CAPX_AER_UC_ERR_SURPRISE_DOWN					(1u << 5)
#define	CAPX_AER_UC_ERR_DATA_LINK_PROTOCOL				(1u << 4)

#define	CAPX_AER_UC_ERR_VALID_BITS \
		((capx_aer_mask_t) \
		( \
			CAPX_AER_UC_ERR_POISONED_TLP_EGRESS_BLOCKED | \
			CAPX_AER_UC_ERR_TLP_PREFIX_BLOCKED | \
			CAPX_AER_UC_ERR_ATOMIC_OP_EGRESS_BLOCKED | \
			CAPX_AER_UC_ERR_MC_BLOCKED_TLP | \
			CAPX_AER_UC_ERR_INTERNAL | \
			CAPX_AER_UC_ERR_ACS_VIOLATION | \
			CAPX_AER_UC_ERR_UNSUPPORTED_REQUEST | \
			CAPX_AER_UC_ERR_ECRC | \
			CAPX_AER_UC_ERR_MALFORMED_TLP | \
			CAPX_AER_UC_ERR_RECEIVER_OVERFLOW | \
			CAPX_AER_UC_ERR_UNEXPECTED_COMPLETION | \
			CAPX_AER_UC_ERR_COMPLETER_ABORT | \
			CAPX_AER_UC_ERR_COMPLETION_TIMEOUT | \
			CAPX_AER_UC_ERR_FLOW_CONTROL_PROTOCOL | \
			CAPX_AER_UC_ERR_POISONED_TLP | \
			CAPX_AER_UC_ERR_SURPRISE_DOWN | \
			CAPX_AER_UC_ERR_DATA_LINK_PROTOCOL \
		))

/* bit masks for Correctable status and mask registers */
#define	CAPX_AER_C_ERR_HDR_LOG_OFLOW					(1u << 15)
#define	CAPX_AER_C_ERR_INTERNAL							(1u << 14)
#define	CAPX_AER_C_ERR_ADVISORY_NONFATAL				(1u << 13)
#define	CAPX_AER_C_ERR_REPLAY_TIMER_TIMEOUT				(1u << 12)
#define	CAPX_AER_C_ERR_REPLAY_NUM_ROLLOVER				(1u << 8)
#define	CAPX_AER_C_ERR_BAD_DLLP							(1u << 7)
#define	CAPX_AER_C_ERR_BAD_TLP							(1u << 6)
#define	CAPX_AER_C_ERR_RECEIVER							(1u << 0)

#define CAPX_AER_C_ERR_VALID_BITS \
		((capx_aer_mask_t) \
		( \
			CAPX_AER_C_ERR_HDR_LOG_OFLOW | \
			CAPX_AER_C_ERR_INTERNAL | \
			CAPX_AER_C_ERR_ADVISORY_NONFATAL | \
			CAPX_AER_C_ERR_REPLAY_TIMER_TIMEOUT | \
			CAPX_AER_C_ERR_REPLAY_NUM_ROLLOVER | \
			CAPX_AER_C_ERR_BAD_DLLP | \
			CAPX_AER_C_ERR_BAD_TLP | \
			CAPX_AER_C_ERR_RECEIVER \
		))

/* bit masks for Root status register */
#define	CAPX_AER_ROOT_ERR_RECVD_FATAL					(1u << 6)
#define	CAPX_AER_ROOT_ERR_RECVD_NONFATAL				(1u << 5)
#define	CAPX_AER_ROOT_ERR_RECVD_FATAL_FIRST				(1u << 4)
#define	CAPX_AER_ROOT_ERR_RECVD_MULTI_UC_ERR			(1u << 3)
#define	CAPX_AER_ROOT_ERR_RECVD_UC_ERR					(1u << 2)
#define	CAPX_AER_ROOT_ERR_RECVD_MULTI_C_ERR				(1u << 1)
#define	CAPX_AER_ROOT_ERR_RECVD_C_ERR					(1u << 0)

#define CAPX_AER_ROOT_ERR_VALID_BITS \
		( \
			CAPX_AER_ROOT_ERR_RECVD_FATAL | \
			CAPX_AER_ROOT_ERR_RECVD_NONFATAL | \
			CAPX_AER_ROOT_ERR_RECVD_FATAL_FIRST | \
			CAPX_AER_ROOT_ERR_RECVD_MULTI_UC_ERR | \
			CAPX_AER_ROOT_ERR_RECVD_UC_ERR | \
			CAPX_AER_ROOT_ERR_RECVD_MULTI_C_ERR | \
			CAPX_AER_ROOT_ERR_RECVD_C_ERR \
		)

/* the 'capx_aer_root_err_mask_e' type defines which errors to generate an interrupt response to */
typedef enum
{
	capx_aer_root_err_mask_e_FATAL = (1u << 2),
	capx_aer_root_err_mask_e_NONFATAL = (1u << 1),
	capx_aer_root_err_mask_e_CORRECTABLE = (1u << 0),

	capx_aer_root_err_mask_e_ALL = (capx_aer_root_err_mask_e_FATAL | capx_aer_root_err_mask_e_NONFATAL | capx_aer_root_err_mask_e_CORRECTABLE),

} capx_aer_root_err_mask_e;

typedef struct
{
	int_t (*version)(pcie_cap_t cap);
	pci_err_t (*read_uc_err_status)(pcie_cap_t cap, capx_aer_status_t *status);
	pci_err_t (*read_uc_err_mask)(pcie_cap_t cap, capx_aer_mask_t *mask);
	pci_err_t (*read_uc_err_sev)(pcie_cap_t cap, capx_aer_sev_t *severity);
	pci_err_t (*read_c_err_status)(pcie_cap_t cap, capx_aer_status_t *status);
	pci_err_t (*read_c_err_mask)(pcie_cap_t cap, capx_aer_mask_t *mask);
	pci_err_t (*write_uc_err_status)(pcie_cap_t cap, pci_devhdl_t hdl, capx_aer_status_t *status);
	pci_err_t (*write_uc_err_mask)(pcie_cap_t cap, pci_devhdl_t hdl, capx_aer_mask_t *mask);
	pci_err_t (*write_uc_err_sev)(pcie_cap_t cap, pci_devhdl_t hdl, capx_aer_sev_t *severity);
	pci_err_t (*write_c_err_status)(pcie_cap_t cap, pci_devhdl_t hdl, capx_aer_status_t *status);
	pci_err_t (*write_c_err_mask)(pcie_cap_t cap, pci_devhdl_t hdl, capx_aer_mask_t *mask);

	pci_err_t (*multihdr_rec_enable)(pcie_cap_t cap, pci_devhdl_t hdl);
	pci_err_t (*multihdr_rec_disable)(pcie_cap_t cap, pci_devhdl_t hdl);
	pci_err_t (*ecrc_check_enable)(pcie_cap_t cap, pci_devhdl_t hdl);
	pci_err_t (*ecrc_check_disable)(pcie_cap_t cap, pci_devhdl_t hdl);
	pci_err_t (*ecrc_generation_enable)(pcie_cap_t cap, pci_devhdl_t hdl);
	pci_err_t (*ecrc_generation_disable)(pcie_cap_t cap, pci_devhdl_t hdl);

	pci_err_t (*read_log_hdr)(pcie_cap_t cap, capx_aer_log_t *hdr);
	pci_err_t (*read_log_tlp_prefix)(pcie_cap_t cap, capx_aer_log_t *tlp_prefix);

	pci_err_t (*root_err_reporting_enable)(pcie_cap_t cap, pci_devhdl_t hdl, capx_aer_root_err_mask_e err_select);
	pci_err_t (*root_err_reporting_disable)(pcie_cap_t cap, pci_devhdl_t hdl, capx_aer_root_err_mask_e err_select);
	int_t (*read_first_err_pointer)(pcie_cap_t cap);
	pci_err_t (*read_root_err_status)(pcie_cap_t cap, capx_aer_status_t *status);
	pci_err_t (*write_root_err_status)(pcie_cap_t cap, pci_devhdl_t hdl, capx_aer_status_t *status);
	pci_err_t (*read_requester_id)(pcie_cap_t cap, capx_aer_requester_id_t *correctable, capx_aer_requester_id_t *fatal);
	int_t (*read_int_msgnum)(pcie_cap_t cap);

} capx_aer_api_t;

#define AER_API_VERSION		{.major = 1, .minor = 0}

#define AER_API_CALL(_api_, _cap_, args...) \
		do { \
			assert((_cap_) != NULL);	/* bogus 'pci_cap_t' */ \
			assert((_cap_)->api_p != NULL);		/* 'pci_cap_t' not read */ \
			assert(((capx_aer_api_t *)((_cap_)->api_p))->_api_ != NULL);	/* module does not implement API */ \
			const pci_version_t api_version = AER_API_VERSION; \
			(_cap_)->api_version = api_version; \
			return ((capx_aer_api_t *)((_cap_)->api_p))->_api_(_cap_, ## args); \
		} while(0)
/*
 ===============================================================================
 Advanced Error Reporting PCIe extended capability specific API's
*/

/* return the AER capability version. A value of < 0 indicates an error */
static inline int_t capx_aer_version(pcie_cap_t cap)
{
	AER_API_CALL(version, cap);
}

/* ... API's for read/writing uncorrectable error status, mask and severity register values */
static inline pci_err_t capx_aer_read_uc_err_status(pcie_cap_t cap, capx_aer_status_t *status)
{
	AER_API_CALL(read_uc_err_status, cap, status);
}
static inline pci_err_t capx_aer_read_uc_err_mask(pcie_cap_t cap, capx_aer_mask_t *mask)
{
	AER_API_CALL(read_uc_err_mask, cap, mask);
}
static inline pci_err_t capx_aer_read_uc_err_severity(pcie_cap_t cap, capx_aer_sev_t *severity)
{
	AER_API_CALL(read_uc_err_sev, cap, severity);
}
static inline pci_err_t capx_aer_write_uc_err_status(pci_devhdl_t hdl, pcie_cap_t cap, capx_aer_status_t *status)
{
	AER_API_CALL(write_uc_err_status, cap, hdl, status);
}
static inline pci_err_t capx_aer_write_uc_err_mask(pci_devhdl_t hdl, pcie_cap_t cap, capx_aer_mask_t *mask)
{
	AER_API_CALL(write_uc_err_mask, cap, hdl, mask);
}
static inline pci_err_t capx_aer_write_uc_err_severity(pci_devhdl_t hdl, pcie_cap_t cap, capx_aer_sev_t *severity)
{
	AER_API_CALL(write_uc_err_sev, cap, hdl, severity);
}

/* ... API's for reading/writing correctable error status and mask register values */
static inline pci_err_t capx_aer_read_c_err_status(pcie_cap_t cap, capx_aer_status_t *status)
{
	AER_API_CALL(read_c_err_status, cap, status);
}
static inline pci_err_t capx_aer_read_c_err_mask(pcie_cap_t cap, capx_aer_mask_t *mask)
{
	AER_API_CALL(read_c_err_mask, cap, mask);
}
static inline pci_err_t capx_aer_write_c_err_status(pci_devhdl_t hdl, pcie_cap_t cap, capx_aer_status_t *status)
{
	AER_API_CALL(write_c_err_status, cap, hdl, status);
}
static inline pci_err_t capx_aer_write_c_err_mask(pci_devhdl_t hdl, pcie_cap_t cap, capx_aer_mask_t *mask)
{
	AER_API_CALL(write_c_err_mask, cap, hdl, mask);
}

/*
 API to obtain the first error pointer corresponding to the first uncorrectable
 error bit set.
 A return value < 0 should treated as an error condition
 A value of 0 means that the pointer is invalid and there is no log information
*/
static inline int_t capx_aer_read_first_err_pointer(pcie_cap_t cap)
{
	AER_API_CALL(read_first_err_pointer, cap);
}

/* ... API's for configuration and control of the advanced error reporting capability.
 If the device function does not support the specific AER capability, PCI_ERR_ENOTSUP
 will be returned
 If the setting is already in the desired state, PCI_ERR_EALREADY will be returned
 to allow the caller to identify the current setting
*/
static inline pci_err_t capx_aer_multihdr_rec_enable(pci_devhdl_t hdl, pcie_cap_t cap)
{
	AER_API_CALL(multihdr_rec_enable, cap, hdl);
}
static inline pci_err_t capx_aer_multihdr_rec_disable(pci_devhdl_t hdl, pcie_cap_t cap)
{
	AER_API_CALL(multihdr_rec_disable, cap, hdl);
}
static inline pci_err_t capx_aer_ecrc_check_enable(pci_devhdl_t hdl, pcie_cap_t cap)
{
	AER_API_CALL(ecrc_check_enable, cap, hdl);
}
static inline pci_err_t capx_aer_ecrc_check_disable(pci_devhdl_t hdl, pcie_cap_t cap)
{
	AER_API_CALL(ecrc_check_disable, cap, hdl);
}
static inline pci_err_t capx_aer_ecrc_generation_enable(pci_devhdl_t hdl, pcie_cap_t cap)
{
	AER_API_CALL(ecrc_generation_enable, cap, hdl);
}
static inline pci_err_t capx_aer_ecrc_generation_disable(pci_devhdl_t hdl, pcie_cap_t cap)
{
	AER_API_CALL(ecrc_generation_disable, cap, hdl);
}

/*
 ... API's for obtaining the header and TLP prefix logs.
 For the header log, if the first error pointer is not valid, PCI_ERR_ENOENT is
 returned
 For the TLP prefix log, if the device does not support end-to-end TLP's,
 PCI_ERR_ENOTSUP will be returned. If the TLP prefix log is not valid
 (bit 11 of AER register 0x18 is 0) PCI_ERR_ENOENT is returned
*/
static inline pci_err_t capx_aer_read_log_hdr(pcie_cap_t cap, capx_aer_log_t *hdr)
{
	AER_API_CALL(read_log_hdr, cap, hdr);
}
static inline pci_err_t capx_aer_read_log_tlp_prefix(pcie_cap_t cap, capx_aer_log_t *tlp_prefix)
{
	AER_API_CALL(read_log_tlp_prefix, cap, tlp_prefix);
}

/* === the remaining API's apply to Root Ports and Root Complex Event Collectors === */

/* ... API's for enabling/disabling specific error reporting
 The <err_select> is a bit mask of the errors to enable/disable and only the
 specified errors will be modified
*/
static inline pci_err_t capx_aer_root_err_reporting_enable(pci_devhdl_t hdl, pcie_cap_t cap, capx_aer_root_err_mask_e err_select)
{
	AER_API_CALL(root_err_reporting_enable, cap, hdl, err_select);
}
static inline pci_err_t capx_aer_root_err_reporting_disable(pci_devhdl_t hdl, pcie_cap_t cap, capx_aer_root_err_mask_e err_select)
{
	AER_API_CALL(root_err_reporting_disable, cap, hdl, err_select);
}

/* ... API's to read/write the root error status register and obtain the interrupt number associated
 with interrupts enabled with capx_aer_root_err_reporting_enable()
*/
static inline pci_err_t capx_aer_read_root_err_status(pcie_cap_t cap, capx_aer_status_t *status)
{
	AER_API_CALL(read_root_err_status, cap, status);
}
static inline pci_err_t capx_aer_write_root_err_status(pci_devhdl_t hdl, pcie_cap_t cap, capx_aer_status_t *status)
{
	AER_API_CALL(write_root_err_status, cap, hdl, status);
}
/* return the interrupt number for status register changes. A return value < 0 indicates an error */
static inline int_t capx_aer_read_int_msgnum(pcie_cap_t cap)
{
	AER_API_CALL(read_int_msgnum, cap);
}

/* ... API to retrieve error source identification. Either 'capx_aer_requester_id_t' parameter can be NULL */
static inline pci_err_t capx_aer_read_requester_id(pcie_cap_t cap, capx_aer_requester_id_t *correctable, capx_aer_requester_id_t *fatal)
{
	AER_API_CALL(read_requester_id, cap, correctable, fatal);
}




#endif	/* _CAPX_AER_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/pci/mainline/modules/capabilities/public/pci/capx_aer.h $ $Rev: 886338 $")
#endif
