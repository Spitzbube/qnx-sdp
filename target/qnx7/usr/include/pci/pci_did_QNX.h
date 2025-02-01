#ifndef _PCI_DID_QNX_H_
#define _PCI_DID_QNX_H_
/*
 * $QNXLicenseC:
 * Copyright (c) 2019 QNX Software Systems. All Rights Reserved.
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

#include <pci/pci.h>

/*
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

 Assigned Device ID's for vendor PCI_VID_BlackBerry_QNX (0x1C05)

 **IMPORTANT**

 This file is the single point of control for Device ID assignments of QNX
 implemented devices. Since it is a public header, these assignments cannot
 be changed after they have been released

 The file 'pcidatabase.com-tab_delimited.txt' (and any future variants supported
 by the strings module), simply provides a mapping of a given vendors devices
 to a textual description meant for human consumption and IS NOT the
 aforementioned point of control

 If you add a new Device ID to this file, please also add a concise description
 to the relevant database files in the strings module
 Search for "QNX"

 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#define PCI_DID_QNX_qvm_shmem_factory			0x0001
#define PCI_DID_QNX_qvm_host bridge				0x0002
#define PCI_DID_QNX_qvm_dummy					0x0003
#define PCI_DID_QNX_qvm_vpu						0x0004
#define PCI_DID_QNX_qvm_camera					0x0005

#endif	/* _PCI_DID_QNX_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/pci/mainline/lib/public/pci/pci_did_QNX.h $ $Rev: 898423 $")
#endif
