/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

/*
 *  dcmd_sim_nvme.h   Non-portable low-level devctl definitions
 *
*/

#ifndef __DCMD_SIM_NVME_H_INCLUDED
#define __DCMD_SIM_NVME_H_INCLUDED

#ifndef _DEVCTL_H_INCLUDED
 #include <devctl.h>
#endif

#ifndef __DCMD_CAM_H_INCLUDED
 #include <sys/dcmd_cam.h>
#endif

#include <_pack64.h>

struct nvme_cmd {
	_Uint8t		opc;		/* dword 0, opcode */
	_Uint8t		flags;
	_Uint16t	rsvd0;

	_Uint32t	nsid;		/* dword 1, namespace identifier*/

	_Uint32t	rsvd2;		/* dword 2-3 */
	_Uint32t	rsvd3;
	_Uint64t	rsvd4;		/* dword 4-5 */
	_Uint64t	rsvd6_7;	/* dword 6-7 */
	_Uint64t	rsvd8_9;	/* dword 8-9 */

	_Uint32t	cdw10;		/* dword 10-15 */
	_Uint32t	cdw11;
	_Uint32t	cdw12;
	_Uint32t	cdw13;
	_Uint32t	cdw14;
	_Uint32t	cdw15;
} __packed;

struct nvme_cmd_rsp {	
#define NVME_RSP_INVAL					~0
	_Uint32t		cs;                     /* Command Specific */
	_Uint32t		sc;                     /* Status Code */
};

struct nvme_pass_thru {
#define NVME_PT_FLG_ADMIN_CMD	0x01
#define NVME_PT_FLG_DIR_IN		0x02
#define NVME_PT_FLG_DIR_OUT		0x04
	_Uint32t				flags;
	_Uint32t				timeout;		/* timeout in seconds */
	_Uint32t				sglist_cnt;		/* Num of scatter gather list entries */
	_Uint32t				dxfer_len;		/* Data transfer length */
	struct nvme_cmd			cmd;			/* caller specifies only opc, nsid, cdw10-cdw15 */
	struct nvme_cmd_rsp		rsp;			/* completion status */

	_Uint64t				data_ptr;		/* physical address of buffer, or offset */
											/* from start of struct to scatter gather */
											/* list */

	_Uint64t				rsvd[12];
};

struct nvme_stats {
#define NVME_STATS_ACTION_READ				0x0
#define NVME_STATS_ACTION_ENABLE			0x1
#define NVME_STATS_ACTION_DISABLE			0x2
	_Uint32t				action;
	_Uint32t				max_queued;		/* maximum queued requests */

	_Uint64t				min_read_time;	/* minimum read time in ns */
	_Uint64t				min_write_time;	/* minimum write time in ns */
	_Uint64t				max_read_time;	/* maximum read time in ns */
	_Uint64t				max_write_time;	/* maximum write time in ns */

	_Uint64t				rsvd[59];
};

#define DCMD_SIM_NVME_PASS_THRU                __DIOTF(_DCMD_CAM, _SIM_NVME + 1, struct nvme_pass_thru )
#define DCMD_SIM_NVME_STATS                    __DIOTF(_DCMD_CAM, _SIM_NVME + 2, struct nvme_stats )

#include <_packpop.h>

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devb/nvme/public/hw/dcmd_sim_nvme.h $ $Rev: 876978 $")
#endif
