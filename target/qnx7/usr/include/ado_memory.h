/*
 * $QNXLicenseC:
 * Copyright 2007, QNX Software Systems. All Rights Reserved.
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

/*
 *      ado_memory.h
 *	    - Manifests to assist in the debugging of the ado drivers.
 */


#ifndef _ADO_MEMORY_H_INCLUDED_
#define _ADO_MEMORY_H_INCLUDED_


#define	ADO_BUF_DMA_SAFE			(1<<0)
#define	ADO_BUF_CACHE				(1<<20)	/* DO cache the buffer */
#define	ADO_BUF_CACHE_NOPS			(1<<21)	/* DON'T Flush OR Invalidate the buffer cache */

#define	ADO_SHM_DMA_SAFE			ADO_BUF_DMA_SAFE

#ifdef ADO_DEBUG

#define		ado_malloc(s)		ado_malloc_debug(s, __FILE__, __LINE__)
#define		ado_calloc(n,s)		ado_calloc_debug(n, s, __FILE__, __LINE__)
#define		ado_realloc(p,s)	ado_realloc_debug(p, s, __FILE__, __LINE__)
#define		ado_strdup(s)		ado_strdup_debug(s, __FILE__, __LINE__)
#define		ado_free(p)			ado_free_debug(p, __FILE__, __LINE__)

#else

#define		ado_malloc			malloc
#define		ado_calloc			calloc
#define		ado_realloc			realloc
#define		ado_strdup			strdup
#define		ado_free			free

#endif

typedef struct ado_card				ado_card_t;

struct ado_pcm_config;
struct ado_pcm_dmabuf;

#if _NTO_VERSION >= 704
#include <smmu.h>
#endif

/*** function prototypes ****/

/* memory.c */
void ado_memory_dump ( void );
void *ado_malloc_debug ( size_t size , const char *filename , int line );
void *ado_calloc_debug ( size_t n , size_t size , const char *filename , int line );
void *ado_realloc_debug ( void *ptr , size_t size , const char *filename , int line );
char *ado_strdup_debug ( const char *src , const char *filename , int line );
void ado_free_debug ( void *ptr , const char *filename , int line );
void ado_memory_dump ( void );

/* Device register mappings routines (SMMU compatible) */
void *ado_device_mmap ( unsigned long addr , unsigned long size );
int ado_device_munmap ( void *addr , unsigned long size );

/* DMA capable PCM Buffers allocation routines (SMMU compatible) */
void *ado_pcm_buf_alloc ( struct ado_pcm_config *config , size_t size , uint32_t flags );
void *ado_pcm_buf_map ( struct ado_pcm_config *config , off64_t phys_addr , size_t size , uint32_t flags );
void ado_pcm_buf_free ( struct ado_pcm_config *config );

/* Generic physical memory allocation routines (SMMU compatible) */
void *ado_mmap_phys ( ado_card_t *card, unsigned long size, int flags, off64_t *paddr );
int ado_munmap_phys ( ado_card_t *card, void *addr, unsigned long size );

/* Deprecated, use ado_pcm_buf_*() and ado_mmap_phys() */
void *ado_shm_alloc ( size_t size , char *name , uint32_t flags , off64_t *phys_addr );
void *ado_shm_map ( off64_t phys_addr , size_t size , char *name , uint32_t flags );
int ado_shm_free ( void *ptr , size_t size , char *name );
int ado_shm_chown (const char * shmobj_name, uid_t euid, gid_t egid);

#if _NTO_VERSION >= 704
/* SMMU support functions */
int ado_smmu_device_add ( ado_card_t *card, const union smmu_devid *devid );
void ado_smmu_device_remove( ado_card_t *card, const union smmu_devid *devid );
int ado_smmu_mapping_add( ado_card_t *card, off64_t phys_addr, uint32_t size);
void ado_smmu_mapping_remove( ado_card_t *card, off64_t phys_addr, uint32_t size);
#endif

int ado_dma_pool_shmem_map ( char *name, size_t size );
int ado_dma_pool_create ( size_t size );
int ado_dma_pool_info (size_t *size, size_t *free, int *chunks, int nchunks);
int ado_dma_pool_unmap ( char *name );

#endif /* _ADO_MEMORY_H_INCLUDED_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/BC700_6762_sdp704/services/audio/public/include/ado_memory.h $ $Rev: 885778 $")
#endif
