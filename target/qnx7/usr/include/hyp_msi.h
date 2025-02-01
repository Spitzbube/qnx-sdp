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
 * Definitions for MSI vectors management.
 * This is only useful on x86 platforms at this point.
 */

#ifndef __HYP_MSI_H
#define __HYP_MSI_H

enum {
    MAF_CONTIG      = (1u << 31),
    MAF_ALIGN_MASK  = 0xffu,
};

#define MSI_UNKNOWN 0xffffffffu

/**
 * Information about an MSI vector, from the hardware and system points of view.
 */
struct msi_info {
    unsigned    mi_vector;  /**< Hardware vector */
    int         mi_intr;    /**< Value suitable for InterruptAttach() */
};

int     msi_alloc(unsigned __num_vectors, struct msi_info *__vectors, unsigned __flags);
int     msi_free(unsigned __num_vectors, const struct msi_info *__vectors, unsigned __flags);

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/hyp/public/hyp_msi.h $ $Rev: 855720 $")
#endif
