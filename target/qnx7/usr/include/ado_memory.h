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

#ifdef ADO_DEBUG

#define		ado_malloc(s)		ado_malloc_debug(s, __FILE__, __LINE__)
#define		ado_calloc(n,s)		ado_calloc_debug(n, s, __FILE__, __LINE__)
#define		ado_realloc(p,s)	ado_realloc_debug(p, s, __FILE__, __LINE__)
#define		ado_strdup(s)		ado_strdup_debug(s, __FILE__, __LINE__)
#define		ado_free(p)			ado_free_debug(p, __FILE__, __LINE__)

#else

#include <malloc.h>

#define		ado_malloc			malloc
#define		ado_calloc			calloc
#define		ado_realloc			realloc
#define		ado_strdup			strdup
#define		ado_free			free

#endif

typedef struct ado_card				ado_card_t;

struct ado_pcm_config;
struct ado_pcm_dmabuf;

#ifdef HAVE_SMMU_H
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

/**
 * Map device physical memory region into the driver's virtual memory.
 * If smmu is enabled, the device address will also be registered with smmuman.
 *
 * @param card   The audio card the device memory is associated with
 * @param addr   The address of the physical memory region
 * @param size   The size of the physical memory region
 * @return       A pointer to the resultant virtual memory region, or MAP_FAILED if an error occurred (errno is set)
 */
void *ado_device_mmap ( ado_card_t *card, unsigned long addr , unsigned long size );

/**
 * Release device memory region from the driver's virtual memory.
 * If smmu is enabled, the device address will also be deregistered from smmuman.
 *
 * @param card   The audio card the device memory is associated with
 * @param addr   The address of the physical memory region
 * @param size   The size of the physical memory region
 * @return       -1 if an error occurred (errno is set). Any other value indicates success.
 */
int ado_device_munmap ( ado_card_t *card, void *addr , unsigned long size );

/**
 * Map device I/O memory region into the driver's virtual memory (accessible via the in*() and out*() functions).
 * If smmu is enabled, the device address will also be registered with smmuman.
 *
 * @param card   The audio card the device I/O memory is associated with
 * @param addr   The address of the device I/O memory
 * @param size   The size of the device I/O memory
 * @return       A handle to the device's I/O memory, or MAP_DEVICE_FAILED if an error occurs (errno is set).
 */
uintptr_t ado_device_mmap_io ( ado_card_t *card, unsigned long addr , unsigned long size );

/**
 * Release access to device I/O memory
 * If smmu is enabled, the device address will also be deregistered from smmuman.
 *
 * @param card   The audio card the device I/O memory is associated with
 * @param addr   The address of the device I/O memory
 * @param size   The size of the device I/O memory
 * @return       -1 if an error occurred (errno is set). Any other value indicates success.
 */
int ado_device_munmap_io ( ado_card_t *card, uintptr_t addr , unsigned long size );

/**
 * Allocate an audio PCM buffer
 * If smmu is enabled, the memory address will also be registered with smmuman.
 *
 * @param card    The audio card the PCM buffer is associated with
 * @param config  PCM config structure to be populated with PCM buffer info
 * @param size    The size of the PCM buffer to allocate
 * @return        Pointer to PCM buffer, or NULL on failure
 */
void *ado_pcm_buf_alloc ( ado_card_t *card, struct ado_pcm_config *config , size_t size , uint32_t flags );

/**
 * Map and externally allocatd PCM data buffer
 * If smmu is enabled, the memory address will also be registered with smmuman.
 *
 * @param card       The audio card the PCM buffer is associated with
 * @param config     PCM config structure to be populated with PCM buffer info
 * @param phys_addr  Physical address of PCM buffer
 * @param size       The size of the PCM buffer to allocate
 * @return           Pointer to PCM buffer, or NULL on failure
 */
void *ado_pcm_buf_map ( ado_card_t *card, struct ado_pcm_config *config , off64_t phys_addr , size_t size , uint32_t flags );

/**
 * Release an allocated or mapped PCM buffer
 * If smmu is enabled, the memory address will also be deregistered from smmuman.
 *
 * @param card    The audio card the PCM buffer is associated with
 * @param config  PCM config structure describing the PCM buffer
 * @return        Nothing
 */
void ado_pcm_buf_free ( ado_card_t *card, struct ado_pcm_config *config );

/**
 * Map a generic physical memory buffer
 * If smmu is enabled, the memory address will also be registered with smmuman.
 *
 * @param card       The audio card the PCM buffer is associated with
 * @param size       The size of the memory buffer
 * @param flags      Mapping flags
 * @param phys_addr  Pointer to physical address, NULL or a value of 0 for anonymous memory
 * @return           Pointer to memory buffer, or NULL on failure
 */
void *ado_mmap_phys ( ado_card_t *card, unsigned long size, int flags, off64_t *paddr );

/**
 * Release a generic physical memory buffer
 * If smmu is enabled, the memory address will also be deregistered from smmuman.
 *
 * @param card  The audio card the PCM buffer is associated with
 * @param addr  Buffer address, NULL or a value of 0 for anonymous memory
 * @param size  The size of the memory buffer
 * @return      0 on success, or -1 (errno set) on failure
 */
int ado_munmap_phys ( ado_card_t *card, void *addr, unsigned long size );

#ifdef HAVE_SMMU_H
/**
 * Register a DMA device for this card with smmuman.
 * This call is only required for MMIO devices that:
 *      1) Do use system DMA and have internal audio DMA
 *      2) Use system DMA directly without using a DMA library
 *
 * @param card       The audio card the memory mapping is associated with
 * @param devid      Pointer to a smmu_devid
 * @return           -1 (errno set) on failure, 0 on success
 */
int ado_smmu_device_add ( ado_card_t *card, const union smmu_devid *devid );

/**
 * Deregister a DMA device for this card from smmuman.
 * This call is only required for MMIO devices that:
 *      1) Do use system DMA and have internal audio DMA
 *      2) Use system DMA directly without using a DMA library
 *
 * @param card       The audio card the memory mapping is associated with
 * @param devid      Pointer to a smmu_devid
 * @return           Nothing
 */
void ado_smmu_device_remove( ado_card_t *card, const union smmu_devid *devid );

/**
 * Register a memory mapping for this card with smmuman.
 * This call is only required if the memmory was not allocated/mapped with an ado_* call.
 *
 * @param card       The audio card the memory mapping is associated with
 * @param phys_addr  Physical memory address
 * @param size       Size of the mapping
 * @return           -1 (errno set) on failure, >=0 on success
 */
int ado_smmu_mapping_add( ado_card_t *card, off64_t phys_addr, uint32_t size);

/**
 * Deregister a memory mapping for this card with smmuman
 * This call is only required if the memmory was not allocated/mapped with an ado_* call.
 *
 * @param card       The audio card the memory mapping is associated with
 * @param phys_addr  Physical memory address
 * @param size       Size of the mapping
 * @return           Nothing
 */
void ado_smmu_mapping_remove( ado_card_t *card, off64_t phys_addr, uint32_t size);
#endif

int ado_dma_pool_shmem_map ( char *name, size_t size );
int ado_dma_pool_create ( size_t size );
int ado_dma_pool_info (size_t *size, size_t *free, int *chunks, int nchunks);
int ado_dma_pool_unmap ( char *name );

#endif /* _ADO_MEMORY_H_INCLUDED_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/services/audio/public/include/ado_memory.h $ $Rev: 896093 $")
#endif
