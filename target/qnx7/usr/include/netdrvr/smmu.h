/*
 * $QNXLicenseC:
 * Copyright 2019, QNX Software Systems.
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

#ifndef NETWORKING_SMMU_H
#define NETWORKING_SMMU_H

/* Register a PCI device, returns 0 on success, -1 on failure */
int smmu_register_pci_device(unsigned pbus, unsigned pdev, unsigned pfunc);

/* Register an MMIO device, returns 0 on success, -1 on failure */
int smmu_register_mmio_device(_Uint64t addr, unsigned size);

/* Unregister a PCI device, returns 0 on success, -1 on failure */
int smmu_unregister_pci_device(unsigned pbus, unsigned pdev, unsigned pfunc);

/* Unregister an MMIO device, returns 0 on success, -1 on failure */
int smmu_unregister_mmio_device(_Uint64t addr, unsigned size);

/* Map some driver specific memory into smmuman, returns 0 on success, -1 on failure*/
int smmu_map_driver_memory(_Uint64t addr, _Uint64t size);

/* Unmap some driver specific memory from smmuman, returns 0 on success, -1 on failure*/
int smmu_unmap_driver_memory(_Uint64t addr, _Uint64t size);

#endif /* NETWORKING_SMMU_H */
#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/BC700_6762_sdp704/lib/io-pkt/sys/lib/libnetdrvr/public/netdrvr/smmu.h $ $Rev: 882875 $")
#endif
