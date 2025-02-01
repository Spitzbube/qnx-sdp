/*
 * $QNXLicenseC:
 * Copyright 2007, 2014 QNX Software Systems. All Rights Reserved.
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

/** @file
 *
 *    POSIX memory management.
 *
 */

#ifndef __MMAN_H_INCLUDED
#define __MMAN_H_INCLUDED

#ifndef __PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif

#if defined(__SIZE_T)
typedef __SIZE_T	size_t;
#undef __SIZE_T
#endif

#if defined(__MODE_T)
typedef __MODE_T	mode_t;
#undef __MODE_T
#endif

#if defined(__OFF_T)
typedef __OFF_T		off_t;
#undef __OFF_T
#endif

#if defined(__OFF64_T)
typedef __OFF64_T	off64_t;
#undef __OFF64_T
#endif

#if !defined(__EXT_POSIX1_199309) && (defined(__EXT_POSIX1_198808) || defined(__EXT_POSIX1_199009))
# error POSIX Threads needs P1003.1b-1993 or later
#endif

#include <_pack64.h>

/* mmap() flags, 0x00-0xff */
#define MAP_SHARED      0x00000001  /*!< Changes impact underlying object */
#define MAP_PRIVATE     0x00000002  /*!< Changes are private to the address space */
#if defined(__EXT_QNX)
#define _MAP_FUTURE1    0x00000004  /*!< Reserved for future public flag */
#define _MAP_FUTURE2    0x00000008  /*!< Reserved for future public flag */
#endif
#define MAP_FIXED       0x00000010  /*!< User assigns address   */
#if defined(__EXT_QNX)
# define MAP_ELF        0x00000020  /*!< Patch ELF relocations  */
# define MAP_NOSYNCFILE 0x00000040  /*!< Memory not synced to file */
# define MAP_LAZY       0x00000080  /*!< Allow lazy mapping     */
#endif
/* mmap() protection bits 0x100-0xf00 */
#define PROT_NONE       0x00000000
#define PROT_READ       0x00000100  /*!< Pages can be read      */
#define PROT_WRITE      0x00000200  /*!< Pages can be written   */
#define PROT_EXEC       0x00000400  /*!< Pages can be executed  */
#if defined(__EXT_QNX)
# define PROT_NOCACHE   0x00000800  /*!< Pages are not cached   */
# define PROT_MASK      0x00000f00
#endif
/* mmap() flags, 0x01000-0x0ff000 */
#if defined(__EXT_QNX)
# define MAP_STACK      0x00001000  /*!< Memory used for stack  */
# define MAP_BELOW      0x00002000  /*!< Allocate below hint    */
# define MAP_NOINIT     0x00004000  /*!< This flag has no effect, to be deprecated */
# define _MAP_FUTURE3   0x00008000  /*!< Reserved for future public flag */
# define MAP_PHYS       0x00010000  /*!< Map physical memory    */
# define MAP_NOINHERIT  0x00020000  /*!< Do not duplicate mapping on fork */
# define MAP_HANDLE     0x00040000  /*!< Map a shmem handle, not an FD */
#endif
#if defined(__EXT_UNIX_MISC) || defined(__EXT_QNX)
# define MAP_ANON       0x00080000  /*!< Anonymous memory       */
# define MAP_ANONYMOUS  MAP_ANON
#endif

#if defined(__EXT_QNX)
/*
 * mmap() reserved flags 0x00100000-0xfff00000.
 * The following bits are reserved for internal system use only and cannot be
 * used to pass information to mmap(). Any such attempt results in mmap()
 * returning EINVAL.
 * MAP_SYSRAM is public as it can be returned when reporting page informtion.
 */
# define _MAP_RESERVE1  0x00100000  /*!< Internal flag          */
# define _MAP_RESERVE2  0x00200000  /*!< Internal flag          */
# define _MAP_RESERVE3  0x00400000  /*!< Internal flag          */
# define _MAP_RESERVE4  0x00800000  /*!< Internal flag          */
# define MAP_SYSRAM     0x01000000  /*!< System ram             */
# define _MAP_RESERVE5  0x02000000  /*!< Internal flag          */
# define _MAP_RESERVE6  0x04000000  /*!< Internal flag          */
# define _MAP_RESERVE7  0x08000000  /*!< Internal flag          */
# define _MAP_RESERVE8  0x10000000  /*!< Internal flag          */
# define _MAP_RESERVE9  0x20000000  /*!< Internal flag          */
# define _MAP_RESERVE10 0x40000000  /*!< Internal flag          */
# define _MAP_RESERVE11 0x80000000  /*!< Internal flag          */

# define MAP_RESERVMASK 0xfff00000

/*
 * The following flags indicate the backing state when reporting page
 * information. These can overlap with the reserved mask, which gets cleared
 * when mmap() flags are reported.
 */
# define PG_MODIFIED     0x00100000     /*!< Pages modified */
# define PG_REFERENCED   0x00200000     /*!< Pages referenced */
# define PG_HWMAPPED     0x00400000     /*!< Pages are present and mapped */
# define PG_MASK         0x00700000     /*!< Mask for page bits */
#endif

/* Sharing type: must specify exactly one of MAP_PRIVATE and MAP_SHARED */
#if defined(__EXT_QNX)
# define MAP_TYPE       (MAP_PRIVATE | MAP_SHARED)  /*!< Mask for share type    */
#endif

/* mmap() flags defined for compatibility and have no effect */
#if defined(__EXT_UNIX_MISC) || defined(__EXT_QNX)
# define MAP_FILE       0x00000000
#endif
#if defined(__EXT_UNIX_HIST)
# define MAP_NORESERVE  0x00000000
# define MAP_RENAME     0x00000000
#endif

/* flags for msync() */
#define MS_ASYNC                0x0000001   /*!< Flush modified area to storage (Don't wait) */
#define MS_SYNC                 0x0000002   /*!< Flush modified area to storage (Return when complete) */
#define MS_INVALIDATE           0x0000004   /*!< Invalidate area (next access will retrieve from storage) */
#if defined(__EXT_QNX)
# define MS_INVALIDATE_LOCKED	0x00100000   /*!< Invalidate cached data on locked range by refreshing its contents */
# define MS_INVALIDATE_ICACHE   0x01000000   /*!< If ::PROT_EXEC, force execution from cache or storage */
# define MS_CACHE_ONLY          0x02000000   /*!< Only operate on CPU cache */
# define MS_CLEAN_ONLY          0x04000000   /*!< Only operate on clean pages */
# define MS_INVALIDATE_RESIZE	0x40000000   /*!< Internal use only for cache resize */
# define MS_CACHE_EXEC          0x80000000   /*!< Internal use only for mprotect with PROT_EXEC */
#endif

/* flags for mlockall() */
#define MCL_CURRENT     0x000000001
#define MCL_FUTURE      0x000000002

#define MAP_FAILED      ((void *)-1)

#if defined(__EXT_POSIX1_200112)    /* Approved 1003.1d D14 */
# define POSIX_MADV_NORMAL       0   /*!< No advice to give */
# define POSIX_MADV_SEQUENTIAL   1   /*!< Sequentialy from lower to higher addresses */
# define POSIX_MADV_RANDOM       2   /*!< Random order */
# define POSIX_MADV_WILLNEED     3   /*!< Expects to access specified range */
# define POSIX_MADV_DONTNEED     4   /*!< Will not access specified range */
#if defined(__EXT_QNX)
# define POSIX_MADV_DISCARD_NP   5   /*!< Discard memory */
#endif
#endif

#if defined(__EXT_QNX)
# define NOFD                (-1)
# define MAP_DEVICE_FAILED   ((_Uintptrt)MAP_FAILED)
#endif

__BEGIN_DECLS

#if defined(__EXT_QNX)

#if defined(__ARM__) \
 || defined(__X86_64__) \
 || defined(__X86__)
	#define __PAGESIZE 0x1000
#elif defined(__aarch64__)
	extern size_t __page_size;
	#define __PAGESIZE (__page_size)
#else
    #error PAGESIZE not configured for system
#endif

# define UNMAP_INIT_REQUIRED	0x00000001
# define UNMAP_INIT_OPTIONAL	0x00000002

extern void *_mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, off64_t __off, unsigned __align, void **__base, size_t *__size);
extern void *_mmap2(void *__addr, size_t __len, int __prot, int __flags, int __fd, off64_t __off, unsigned __align, size_t __preload, void **__base, size_t *__size);
extern void *_mmap_elf(void * __addr, size_t __len, int __prot, int __flags, int __fd, off64_t __off, _Uintptrt __p_vaddr);
extern _Uintptrt mmap_device_io(size_t __len, _Uint64t __io);
extern void *mmap_device_memory(void *__addr, size_t __len, int __prot, int __flags, _Uint64t __physical);
extern int munmap_device_io(_Uintptrt __io, size_t __len);
extern int munmap_device_memory(void *__addr, size_t __len);
extern int munmap_flags(void *__addr, size_t __len, unsigned __flags);

extern int _mmap64_r(void *__addr, size_t __len, int __prot, int __flags, int __fd, off64_t __off, size_t __align, size_t __preload, void **__map, void **__base, size_t *__size);
extern int mlock_r(__const void *__addr, size_t __len);
extern int mlockall_r(int __flags);
extern int mprotect_r(void *__addr, size_t __len, int __prot);
extern int msync_r(void *__addr, size_t __len, int __flags);
extern int munlock_r(__const void *__addr, size_t __len);
extern int munlockall_r(void);
extern int munmap_r(void *__addr, size_t __len, unsigned __flags);

extern int getpagesizes(size_t __pagesize[], int __nelem);
extern int getpagesizes64(_Uint64t __pagesize[], int __nelem);
#endif

#if defined(__EXT_POSIX1_200112)	/* Approved 1003.1d D14 */
extern int posix_madvise(void *__addr, size_t __len, int __advice);
#endif

#ifdef __EXT_LF64SRC
extern void *mmap64(void *__addr, size_t __len, int __prot, int __flags, int __fd, off64_t __off) __ALIASOFF("mmap");
# if defined(__EXT_QNX)
extern int mmap64_r(void *__addr, size_t __len, int __prot, int __flags, int __fd, off64_t __off, void **__map) __ALIASOFF("mmap_r");
# endif
# if defined(__EXT_LF64ALIAS) && !defined(__ALIAS_ATTRIBUTE)
#  error ALIAS not configured for compiler: mmap
# endif
#endif
extern void *mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, off_t __off) __ALIAS64("mmap64");
#if defined(__EXT_QNX)
extern int mmap_r(void *__addr, size_t __len, int __prot, int __flags, int __fd, off_t __off, void **__map) __ALIAS64("mmap64_r");
#endif
#ifdef __EXT_FUNCALIAS64
# ifdef __ALIAS_ATTRIBUTE
/* Use the __ALIAS64 define */
# else
static __inline void * __attribute__((__unused__)) mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, off_t __off) {
    return mmap64(__addr, __len, __prot, __flags, __fd, __off);
}
#  if defined(__EXT_QNX)
static __inline int __attribute__((__unused__)) mmap_r(void *__addr, size_t __len, int __prot, int __flags, int __fd, off_t __off, void **__map) {
    return mmap64_r(__addr, __len, __prot, __flags, __fd, __off, __map);
}
#  endif
# endif
#endif
extern int mprotect(void *__addr, size_t __len, int __prot);
extern int mlock(__const void * __addr, size_t __len);
extern int mlockall(int __flags);
extern int msync(void *__addr, size_t __len, int __flags);
extern int munmap(void *__addr, size_t __len);
extern int munlock(__const void *__addr, size_t __len);
extern int munlockall(void);

extern int shm_open(__const char *__path, int __oflag, mode_t __mode);
extern int shm_unlink(__const char *__path);

#if defined(__EXT_POSIX1_200112)		/* Approved 1003.1j D10 */

# define POSIX_TYPED_MEM_ALLOCATE           0x0001
# define POSIX_TYPED_MEM_ALLOCATE_CONTIG    0x0002
# define POSIX_TYPED_MEM_MAP_ALLOCATABLE    0x0004
extern int posix_typed_mem_open(__const char *__name, int __oflag, int __tflag);

struct __posix_typed_mem_info64 {
    _Uint64t            posix_tmi_length;
    _Uint64t            __posix_tmi_total;
    _Uint32t            __reserved[2];
};

struct posix_typed_mem_info {
# if __SIZE_BITS__ == 32
#  if defined(__LITTLEENDIAN__)
    size_t        posix_tmi_length;
    _Uint32t            __posix_tmi_length_hi;
#  elif defined(__BIGENDIAN__)
    _Uint32t            __posix_tmi_length_hi;
    size_t        posix_tmi_length;
#  else
#   error endian not configured for system
#  endif
#  if defined(__EXT_QNX)
/**@details
 * POSIX defines only the posix_tmi_length field and explicitly does not allow
 * posix_tmi_length_hi to be visible, so only define it as a QNX extension.
 */
#   define posix_tmi_length_hi	__posix_tmi_length_hi
#  endif
# elif __SIZE_BITS__ == 64
    size_t        posix_tmi_length;
# else
#  error Not configured for value of __SIZE_BITS__
# endif
    _Uint64t            __posix_tmi_total;
    _Uint32t            __reserved[2];
};
extern int posix_typed_mem_get_info(int __fd, struct posix_typed_mem_info *__info);

# ifdef __EXT_LF64SRC
extern int posix_mem_offset64(__const void *__addr, size_t __len, off64_t *__off, size_t *__contig_len, int *__fd) __ALIASOFF("posix_mem_offset");
#  if defined(__EXT_LF64ALIAS) && !defined(__ALIAS_ATTRIBUTE)
#   error ALIAS not configured for compiler: posix_mem_offset
#  endif
# endif

extern int posix_mem_offset(__const void *__addr, size_t __len, off_t *__off, size_t *__contig_len, int *__fd) __ALIAS64("posix_mem_offset64");

# ifdef __EXT_FUNCALIAS64
#  ifdef __ALIAS_ATTRIBUTE
/* Use the __ALIAS64 define */
#  else
static __inline int __attribute__((__unused__)) posix_mem_offset(__const void *__addr, size_t __len, off_t *__off, size_t *__contig_len, int *__fd) {
    return posix_mem_offset64(__addr, __len, __off, __contig_len, __fd);
}
#  endif
# endif
#endif

#if defined(__EXT_QNX)		/* 1003.1j D5, used only for getting physical addresses */
# ifdef __EXT_LF64SRC
extern int mem_offset64(__const void *__addr, int __fd, size_t __len, off64_t *__off, size_t *__contig_len) __ALIASOFF("mem_offset");
#  if defined(__EXT_LF64ALIAS) && !defined(__ALIAS_ATTRIBUTE)
#   error ALIAS not configured for compiler: mmap
#  endif
# endif

extern int mem_offset(__const void *__addr, int __fd, size_t __len, off_t *__off, size_t *__contig_len) __ALIAS64("mem_offset64");
# ifdef __EXT_FUNCALIAS64
#  ifdef __ALIAS_ATTRIBUTE
/* Use the __ALIAS64 define */
#  else
static __inline int __attribute__((__unused__)) mem_offset(__const void *__addr, int __fd, size_t __len, off_t *__off, size_t *__contig_len) {
    return mem_offset64(__addr, __fd, __len, __off, __contig_len);
}
#  endif
# endif
#endif

#if defined(__EXT_QNX)		/* From posix 1003.1j D5 */
/* From posix 1003.1j D5 !!!!!!!!!!!!! USE posix_typed_* calls instead !!!!!!!!! */

struct mem_info {
# if __SIZE_BITS__ == 32
#  if defined(__LITTLEENDIAN__)
    size_t              mi_length;
    _Uint32t            mi_length_hi;
#  elif defined(__BIGENDIAN__)
    _Uint32t            mi_length_hi;
    size_t              mi_length;
#  else
#   error endian not configured for system
#  endif
# elif __SIZE_BITS__ == 64
    size_t              mi_length;
# else
#  error Not configured for number of INT bits
# endif
    _Uint32t            mi_reserved[4];
};

# define mem_get_info(__fd, __flags, __info) \
	posix_typed_mem_get_info((__fd), (struct posix_typed_mem_info *)(__info))
#endif

#if defined(__EXT_QNX)

# define TYPEMEM_BACKSTORE   "/backstore"        /*!< Memory that can be swapped out to slower storage */
# define TYPEMEM_SYSRAM      "/sysram"           /*!< System memory pool */
# define TYPEMEM_DEVICE      "/device"           /*!< Devices mapping areas (e.g. video buffers) */

# define TYPEMEM_SYSRAM_ISA  "/sysram.isa"       /*!< X86 system memory below 16M (for DMA) */
# define TYPEMEM_SYSRAM_DMA  "/sysram.dma"       /*!< X86 memory below 16M not crossing 64K boundary. */
# define TYPEMEM_DEVICE_ISA  "/device.isa"       /*!< X86 device memory below 16M (Usually 640K-1M) */

# define SHM_ANON			((char *)-1)

extern int shm_ctl(int __fd, int __flags, _Uint64t __physical, _Uint64t __size);
extern int shm_ctl_special(int __fd, int __flags, _Uint64t __physical, _Uint64t __size, unsigned __special);

# define SHMCTL_ANON         0x00000001          /*!< Allocate anon memory for object */
# define SHMCTL_PHYS         0x00000002          /*!< Providing physical memory for object */
# define SHMCTL_SEAL         0x00000004          /*!< Only the creator can modify the object's layout */
# define SHMCTL_REVOCABLE    0x00000008          /*!< physical mapping is revocable by creator */
# define _SHMCTL_GLOBAL      0x00000010          /*!< Hint that object is globally visible */
static unsigned const SHMCTL_GLOBAL __attribute__((deprecated)) = _SHMCTL_GLOBAL;
# define _SHMCTL_PRIV        0x00000020          /*!< Hint that accessing object requires privilege */
static unsigned const SHMCTL_PRIV __attribute__((deprecated)) = _SHMCTL_PRIV;
# define _SHMCTL_LOWERPROT   0x00000040          /*!< Hint that access to this object can use lower protection in exchange for better performance */
static unsigned const SHMCTL_LOWERPROT __attribute__((deprecated)) = _SHMCTL_LOWERPROT;
# define SHMCTL_LAZYWRITE    0x00000080          /*!< Hint that mapping of this object could use lazy-writing mechanisms */
# define _SHMCTL_HIGHUSAGE   0x00000200          /*!< Hint that the object is high usage and the system should use limited resources for better performance */
static unsigned const SHMCTL_HIGHUSAGE __attribute__((deprecated)) = _SHMCTL_HIGHUSAGE;
# define SHMCTL_LAZY         0x00000400          /*!< Delay actually allocating memory until first reference */
# define SHMCTL_REPEAT       0x00000800          /*!< Extends a SHMCTL_PHYS call with a stride and length pattern */
# define _SHMCTL_NODEFRAG    0x00001000          /*!< Return ENOMEM rather than wait for defragmentation */
static unsigned const SHMCTL_NODEFRAG __attribute__((deprecated)) = _SHMCTL_NODEFRAG;
# define SHMCTL_TYMEM        0x00002000          /*!< Allocate from a typed-memory object */
# define SHMCTL_GET_FLAGS    0x00004000          /*!< Query existing flags */
# define SHMCTL_HAS_SPECIAL  0x00008000

# define SHMCTL_FLAG_MASK   0x0000ffff

extern int _free_stack_pages(void);
extern int _free_stack_pages_max(size_t __max_free);

#if defined(__PID_T)
typedef __PID_T		pid_t;
#undef __PID_T
#endif

typedef _Uint64t    shm_handle_t;
int shm_create_handle(int __fd, pid_t __pid, int __flags, shm_handle_t *__handlep, unsigned __options);

#define SHM_CREATE_HANDLE_OPT_NOFD       0x00000001U // handle to be used directly, cannot open an fd with it
#define SHM_CREATE_HANDLE_OPT_VALID_MASK 0x00000001U

int shm_open_handle(shm_handle_t __handle, int __flags);
int shm_delete_handle(shm_handle_t __handle);
int shm_revoke(int __fd, pid_t __pid);

extern void *mmap_handle(void *__addr, size_t __len, int __prot, int __flags, shm_handle_t __handle, off_t __off);

#endif

#include <_packpop.h>

__END_DECLS

#endif


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/c/public/sys/mman.h $ $Rev: 913485 $")
#endif
