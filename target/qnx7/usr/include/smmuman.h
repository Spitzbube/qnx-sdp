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
 * System memory management unit manager interface definitions
 */

#ifndef _SMMUMAN_H_INCLUDED
#define _SMMUMAN_H_INCLUDED

#include <smmu.h>

/********************************************
 *
 * Interface between libsmmu and smmuman
 *
 *******************************************/

#define SMMU_PATH	"/dev/smmuman"

enum smmu_msgs {
	_SMMU_OBJECT_CREATE,
	_SMMU_OBJECT_DESTROY,
	_SMMU_ADD_DEVICE,
	_SMMU_ADD_MAPPING,
	_SMMU_REPORT_RESERVED,
	_SMMU_SAFETY,
};

struct smmu_msg_object_create {
	unsigned	flags;
};

struct smmu_msg_object_destroy {
	unsigned	obj_name;
};

struct smmu_msg_add_device {
	unsigned			obj_name;
	unsigned			devid_len;
	unsigned			spare[4];
	union smmu_devid	devid;
};

struct smmu_msg_add_mapping {
	_Uint64t	target;
	unsigned	obj_name;
	unsigned	flags;
	pid_t		pid;
	unsigned	num_entries;
	unsigned	spare[4];
	struct smmu_map_entry entries[];
};


struct smmu_msg_report_reserved {
	unsigned			offset;
	unsigned			nresp;
	unsigned			devid_len;
	unsigned			spare[3];
	union smmu_devid	devid;
};



#if defined(VARIANT_safety)
	#if !defined(WANT_SAFETY)
		#define WANT_SAFETY
	#endif
#endif

enum {
	/** Not the safety version of the smmu support file */
	SMMU_SAFETY_NO = 0x55555555,
	/** Is the safety version of the smmu support file */
	SMMU_SAFETY_YES = 0xaaaaaaaa,
	/** Selected version of safety for the smmu support file */
#if defined(WANT_SAFETY)
	SMMU_SAFETY_SELECTED 	= SMMU_SAFETY_YES
#else
	SMMU_SAFETY_SELECTED 	= SMMU_SAFETY_NO
#endif
};

/********************************************
 *
 * Interface between smmuman and its drivers
 *
 *******************************************/

/**
 * The smmu interface ABI version.
 * This number should be changed every time the ABI between smmuman and the
 * loaded support routines changes, to make smmuman error out upon loading a
 * support file of the wrong version.
 * Changing the size of a structure exposed by the public smmuman headers,
 * the prototype of a function or removing a function are all qualifying
 * changes.
 */
#define SMMU_SUPPORT_ABI    2018080701u


struct smmu_unit;


/*
 * These three types are support implementation specific. The generic code
 * treats them as cookies.
 */
struct smmu_support_unit;
struct smmu_support_map;
struct smmu_support_connect;


/*
 * Factory for creating instances of a particular SMMU implementation
 */
struct smmu_factory {
	/** version of support routines supported (SMMU_SUPPORT_ABI) */
	unsigned				version;
	/** number of bytes needed for a smmu_support_unit */
	unsigned				unit_size;
    /** pointer to next entry in list of structures */
    struct smmu_factory		*next;
    /** name of smmu hardware type */
    const char				*name;
	/** function to initialize a particular SMMU type */
    void					(*support_init)(const char *parm);
	/** tell the support code that we're done configuration */
    void					(*config_done)(int have_units);
	/** function to initialize a particular SMMU unit */
	void					(*unit_init)(struct smmu_support_unit *ssup, const void *parm);
	/** function to finalize a particular SMMU unit */
    void					(*unit_fini)(struct smmu_support_unit *ssup);
	/** create a mapping structure */
	struct smmu_support_map	*(*map_create)(void);
	/** set up/remove a mapping */
	int						(*mapping)(struct smmu_support_map *smp, unsigned prot, _Uint64t src, _Uint64t tgt, _Uint64t len);
	/** destroy a mapping structure */
	void					(*map_destroy)(struct smmu_support_map *smp);
	/** prepare to handle connections between a unit and a devid */
	struct smmu_support_connect *(*connect_init)(struct smmu_support_unit *ssup, const union smmu_devid *devid, const char *parm);
	/** connect the devid in a particular unit to a mapping structure */
	int						(*connect)(struct smmu_support_unit *ssup, struct smmu_support_connect *sscp, const union smmu_devid *devid, struct smmu_support_map *smp);
	/** disconnect the devid in a particular unit from a mapping structure */
	void					(*disconnect)(struct smmu_support_unit *ssup, struct smmu_support_connect *sscp, const union smmu_devid *devid, struct smmu_support_map *smp, int noaccess);
	/** is this the safety version of the support routines or not */
	unsigned				safety;
};

struct smmu_unit	*smmu_unit_add(const struct smmu_factory *factory, const char *name, const void *parm);

void	smmu_device_add_unit(const union smmu_devid *devid, struct smmu_unit *unit, struct smmu_support_connect *sscp);
void	smmu_device_add_reserved(const union smmu_devid *devid, _Uint64t start, _Uint64t len);
int		smmu_device_find(const struct smmu_support_unit *ssup, const struct smmu_support_connect *sscp, union smmu_devid *devid);

int		smmu_msgblock_oneshot(unsigned mult);
int		smmu_msgblock_start(unsigned mult);
void	smmu_msgblock_stop(void);


void smmu_report(unsigned severity, const char *fmt, ...);

struct smmu_status;
void smmu_register_fault(unsigned vector, int (*handler)(void *data, struct smmu_status *ssp), void *data);

/** flags to control qvm_parse_num() operation */
enum smmu_parse_num_flags {
	/** No flags specified */
	SPNF_NONE			= 0x00,
	/** Allow number to be scaled by 'k', 'm', 'g' suffixes */
	SPNF_ALLOW_SUFFIXES	= 0x01,
	/** Allow a negative sign on the number */
	SPNF_ALLOW_NEGATIVE	= 0x02,
	/** Allow trailing characters following the number */
	SPNF_ALLOW_TRAILING = 0x04,
	/** Interpret the number as base 16 */
	SPNF_HEX 			= 0x08
};

const char *smmu_parse_num(const char *str, enum smmu_parse_num_flags spnflags, _Uint64t *nump);

void *smmu_load_support(const char *prefix, const char *name);
unsigned smmu_make_non_safety(void);

union smmu_setvar_value {
	_Uint64t	num;
	const void	*ptr;
};

enum smmu_setvar_type {
	SSVT_BOOL,
	SSVT_NUM,
	SSVT_PADDR,
	SSVT_STRING,
	SSVT_NUM_TYPES,
};

enum smmu_setvar_context {
	SSVC_GLOBAL,
	SSVC_NUM_CONTEXTS,
};

struct smmu_setvar_info {
	struct smmu_setvar_info	*next;
	const char 				*name;
	void					(*set)(void *ctx, union smmu_setvar_value val);
	enum smmu_setvar_context context;
	enum smmu_setvar_type	type;
};

void smmu_register_setvar(struct smmu_setvar_info *ssip);


union acpi_generic_u;
const union acpi_generic_u *smmu_acpi_find_table(unsigned table_name, const union acpi_generic_u *start_search);

/* provided by the SMMU driver */
extern struct smmu_factory smmu_support;
#endif


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/BC700_6762_sdp704/services/smmu/public/smmuman.h $ $Rev: 888171 $")
#endif
