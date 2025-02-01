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
#ifndef __WIPE_H__
#define __WIPE_H__

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <hw/dcmd_sim_sdmmc.h>

/*  If the target is a filesystem, attempt to unmount the filesystem prior to
    wiping. The default behavior is to return an error if the target filesystem
    is mounted.
*/
#define WIPE_FLAG_UNMOUNT           (0x00000001u)

/*  Allow the wipe operation to be performed insecurely. This option will enable
    using an insecure version of the requested method. If a method is not
    capable of performing the operation securely, this flag must be set in order
    to use it. The default behavior is to always perform the requested operation
    securely.
*/
#define WIPE_FLAG_INSECURE          (0x00000002u)

/*  Perform the wipe asynchronously. The default behavior is to perform the wipe
    synchronously on the programs main thread.
*/
#define WIPE_FLAG_ASYNC             (0x00000004u)

/*  If the wipe method is WIPE_METHOD_WRITE, attempt to use direct i/o. This
    flag is ignored for all other wipe methods.
*/
#define WIPE_FLAG_DIRECTIO          (0x00000008u)

/*  Enable test mode, which will run through the entire wipe process but not
    actually issue of any of the commands that will destroy (wipe) the data.
*/
#define WIPE_FLAG_TESTMODE          (0x00000010u)

/*  If the wipe scope is WIPE_SCOPE_FS, wipe free space instead of the
    filesystem data contents.
*/
#define WIPE_FLAG_FREESPACE         (0x00000020u)

/*  Bypass io-blk cache coherency operations. By default, io-blk cache coherency
    is enforced by invalidating any cache buffers associated with the wipe
    target range. The use of this option omits the cache coherency operation and
    will generally allow any cached data to be accessed following the wipe.
    Extreme caution should be used when specifying this flag, as it can lead to
    both insecure and sometimes dangerous side effects.
*/
#define WIPE_FLAG_CACHE_INCOHERENT  (0x00000040u)

/*  Enumerate the number of device units (usually sectors) that will be wiped.
    This will execute a first pass through the configured iterator and count all
    of the units to be wiped. Note that depending on configuration of the wipe
    actors for aligned and unaligned portions, the total number of device units
    may exceed either of the total number of units on the target or the total
    number of units to wipe. This is due to the fact that some wipe actors break
    the operation into two parts, marking the units and committing the marked
    units.
*/
#define WIPE_FLAG_ENUMERATE         (0x00000080u)

/*  The mask of the flags field describing the request verbosity.
*/
#define WIPE_FLAG_VERBOSITY_MASK    (0xFF000000u)

/*  The number of bits to shift to get the verbosity into or out of the flags.
*/
#define WIPE_FLAG_VERBOSITY_SHIFT   (24)

/*  The mask of valid flags.
*/
#define WIPE_FLAG_MASK              (0xFF0000FFu)

/*  Describes the scope of data that is to be wiped.

    Example disk layout:
    --------------------------------------------------------------------------
    |          |       f,p,d       | p,d |        p,d         |       d      |
    --------------------------------------------------------------------------
    |<- boot ->|
               |<---- "/base" ---->|
               |<------  /dev/hd0 ------>|
                                         |<---- /dev/hd1 ---->|
                                                              |<-- unused -->|
    |<------------------------------- /dev/emmc ---------------------------->|
*/
typedef enum wipe_scope {
    /*  Specifies only the filesystem and data regions owned by the filesystem.
        Regions marked with 'f' above are affected/selectable.
    */
    WIPE_SCOPE_FS,

    /*  Specifies a partition which may contain a number of filesysems.
        Regions marked with 'p' above are affected/selectable.
    */
    WIPE_SCOPE_PARTITION,

    /*  Specifies an entire device which may contain a number of partitions.
        Regions marked with 'd' above are affected.
    */
    WIPE_SCOPE_DEVICE,

    /*  Specifies a single file.
    */
    WIPE_SCOPE_FILE,

    /*  The number of wipe scopes.
    */
    WIPE_SCOPE_N
} wipe_scope_t;

/*  Describes the available methods to wipe the data.
*/
typedef enum wipe_method {
    /*  Wipe by writing all data with zeroes.
    */
    WIPE_METHOD_WRITE,

    /*  Wipe by using the hardware's 'erase' functionality. The contents of the
        data blocks will be set to the device's erase value (usually 0 or 1).
        Units of erasure are generally an in 'erase block groups' defined by the
        hardware.
    */
    WIPE_METHOD_ERASE,

    /*  Wipe by using the hardware's 'trim' functionality. This is generally
        identical in behavior to the 'erase' functionality except that erasure
        units are in 'write block groups', which are generally smaller than
        erase block groups.
    */
    WIPE_METHOD_TRIM,

    /*  Wipe by using the hardware's 'discard' functionality. This marks blocks
        as being no longer used and the device may or may not wipe the contents
        of discarded blocks in the background. The hardware provides no
        guarantee that existing data within a block will be erased and so this
        method is insecure.
    */
    WIPE_METHOD_DISCARD,

    /*  Do nothing. This is intended to be used by informational actions.
    */
    WIPE_METHOD_NOOP,

    /*  The number of wipe methods.
    */
    WIPE_METHOD_N
} wipe_method_t;

/*  Describes the scope an action has on its input.
*/
typedef enum action_scope {
    /*  Represents the action to take for the data portion that does not satisfy
        the wipe alignment requirement.
    */
    ACTION_UNALIGNED,

    /*  Represents the action to take for the data portion that satisfies the
        wipe alignment requirement.
    */
    ACTION_ALIGNED,

    /*  The number of action elements in this enumeration.
    */
    ACTION_N
} action_scope_t;

/*  Describes the representation of an address.
*/
typedef enum address_type {
    /*  Describes a logical address.
    */
    ADDRESS_LOGICAL,

    /*  Describes a physical address.  A physical address is one that includes the
        partition offset.
    */
    ADDRESS_PHYSICAL,

    /*  Describes an absolute address. An absolute address is one that that
        would be used by an underlying devb driver. These types of addresses
        include both the partition offset and the base address relative to the
        physical device in which it resides.
    */
    ADDRESS_ABSOLUTE,

    /*  The number of offset types in this enumeration.
    */
    ADDRESS_N
} address_type_t;

/*  Indicates the wipe target has at least one filesystem mounted on it.
*/
#define WIPE_TARGET_MOUNTED         (0x00000001u)

/*  Indicates the target is a raw block device.
*/
#define WIPE_TARGET_DEVICE_RAW      (0x00000002u)

/*  Configuration options for all wipe methods.
*/
typedef struct wipe_config {
    uint64_t    unit;
} wipe_config_t;

/*  Wipe target structure. Contains information about the target filesystem,
    partition of device to wipe.
*/
typedef struct wipe_target {
    int                 fd;
    uint32_t            flags;
    uint64_t            slba;
    uint64_t            elba;
    struct stat64       info;
    struct statvfs64    vfs;
    SDMMC_DEVICE_INFO   sdmmc;
    char                path[PATH_MAX+1];
    char                mntpt[PATH_MAX+1];
} wipe_target_t;

/*  Wipe request structure. Contains all of the information necessary to perform
    a wipe request.
*/
typedef struct wipe_request {
    uint32_t        flags;
    uint32_t        scope;
    uint32_t        method;
	uint32_t        pad;
    wipe_target_t   target;
    wipe_config_t   config;
} wipe_request_t;

/*  Wipe action information structure. This contains information required by
    wipe actions that can vary across different wipe requests.
*/
typedef struct wipe_action_info {
    uint64_t        alignment;
    uint32_t        addressing;
} wipe_action_info_t;

/*  The action shall queue the target data for wipe. The data shall not be
    wiped.
*/
#define WIPE_ACTION_OP_QUEUE    (0x00000001u)

/*  The action shall commit wipe operations that have previously been queued.
    This performs the actual wipe operation.
*/
#define WIPE_ACTION_OP_COMMIT   (0x00000002u)

/*  The action shall enumerate an extent. The range is not wiped. This is
    an informational action.
*/
#define WIPE_ACTION_OP_ENUM     (0x00000004u)

/*  Performs both the queue and commit operations in one step. The
    implementation may skip the queing operation if it is more effecient to do
    so.
*/
#define WIPE_ACTION_OP_WIPE     ( WIPE_ACTION_OP_QUEUE  \
                                | WIPE_ACTION_OP_COMMIT)

/*  Wipe action argument structure. All wipe actions accept a wipe argument,
    containing context information that is specific to the wipe argument data,
    configuration information that is specific to the wipe action, and action
    info that is specific to the wipe action and wipe request the action is
    being invoked for.
*/
typedef struct wipe_action_arg {
    void               *context;
    void               *config;
    wipe_action_info_t *info;
    uint32_t            opmask;
} wipe_action_arg_t;

/*  Wipe action prototypes.
*/
struct wipe_action;
typedef void (*wipe_action_show_fn_t)(const struct wipe_action * const, const wipe_action_arg_t * const);
typedef int  (*wipe_action_info_fn_t)(const wipe_request_t * const, const struct wipe_action * const, wipe_action_info_t * const);
typedef int  (*wipe_action_impl_fn_t)(const wipe_request_t * const, const struct wipe_action * const, const wipe_action_arg_t * const);

/*  Wipe action structure. A wipe action performs the physical wipe operation on
    the target.
*/
typedef struct wipe_action {
    char                  name[16];     /*  Name of the action.             */
    uint32_t              method;       /*  Wipe method for action.         */
    wipe_action_impl_fn_t wipe;         /*  Wipe action implementation.     */
    wipe_action_info_fn_t info;         /*  Action info discovery function. */
    wipe_action_show_fn_t show;         /*  Prints wipe action description. */
} wipe_action_t;

/*  Wipe implementation (algorithm) prototypes.
*/
struct wipe_algorithm;
typedef int (*wipe_algorithm_fn_t)(const struct wipe_algorithm * const, wipe_request_t * const, void *);
typedef int (*wipe_algorithm_config_fn_t)(const struct wipe_algorithm * const, wipe_request_t * const, void *);

/*  Wipe algorithm structure. Defines an algorithm for wiping a target.
*/
typedef struct wipe_algorithm {
    wipe_algorithm_fn_t        run;
    wipe_algorithm_config_fn_t configure;
} wipe_algorithm_t;

/*  Returns the verbosity associated with a wipe request. See wipe.c for a
    complete description of how this function operates.
*/
extern uint8_t wipe_request_verbosity(
    const wipe_request_t * const request);

/*  Performs the specified wipe action. See wipe.c for a complete description of
    how this function operates.
*/
extern int wipe_action_execute(
    const wipe_request_t * const request,
    const wipe_action_t * const action,
    const wipe_action_arg_t * const arg);

/*  Wipes a target filesystem, partition or device. See wipe.c for a complete
    description of how this function operates.
*/
extern int wipe(
    wipe_request_t * const request);


/*  Initializes a wipe request with default values.

    Parameters
        request - A pointer to the request to initialize.
*/
extern void request_init(
    wipe_request_t *request);

/*  Displays information about a wipe request.

    Parameters
        request - A pointer to the request structure to display information for.
*/
extern void request_display(
    const wipe_request_t * const request);
#endif /* __WIPE_H__ */


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/services/blk/lib/wipe/public/wipe.h $ $Rev: 897915 $")
#endif
