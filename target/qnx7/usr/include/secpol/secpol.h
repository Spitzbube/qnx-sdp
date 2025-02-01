/*
 * $QNXLicenseC:
 * Copyright 2016-2019 QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software.	 Free development
 * licenses are available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others.	 Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#ifndef _SECPOL_SECPOL_H_INCLUDED
#define _SECPOL_SECPOL_H_INCLUDED

#include <stdint.h>
#include <sys/types.h>

#ifdef __QNXNTO__
#include <sys/neutrino.h>
#else
#if __PTR_BITS__ <= 32
    #define _msg_info	_msg_info32
#else
    #define _msg_info	_msg_info64
#endif
struct _msg_info;
#endif

/** @cond */
__BEGIN_DECLS
/** @endcond */


#define SECPOL_INVALID_TYPE 0xffffffff

// Policy id that represents no security policy being in effect
#define SECPOL_POLICY_ID_NO_POLICY	0
// Bit set in the policy id that indicates that the policy is immutable and cannot be changed once loaded.
#define SECPOL_POLICY_ID_IMMUTABLE	0x80000000

#define SECPOL_MAGIC   0x50434553
#define SECPOL_VERSION 1

/** Default security policy file */
#define SECPOL_DEFAULT_POLICY_FILE "/proc/boot/secpol.bin"

/** Blob entry providing location of size of blob within a secpol file.	 An array of these follows the secpol file
 * header.
 */
typedef struct {
	/** Type of data represented by blob */
	uint32_t type;
	/** Offset in file of data */
	uint32_t offset;
	/** Size of data in bytes */
	uint32_t size;
} secpol_blob_t;


/** Header for all secpol files */
typedef struct {
	/** Magic number to identify secpol files */
	uint32_t magic;
	/** File version */
	uint32_t version;
	/** Number of blobs contained in the file */
	uint32_t blob_count;
	/** blob_count blob headers */
	secpol_blob_t blobs[0];
} secpol_file_header_t;

typedef struct secpol_file_s secpol_file_t;

enum secpol_open_flags_e {
	/** Use handle as default when NULL is specified for the handle in other calls. */
	SECPOL_USE_AS_DEFAULT = 1,
};

#ifdef __QNX__

#include <spawn.h>

/**
 * Open a security policy file.	 Most uses of libsecpol require the policy file only once in which
 * case this function can be omitted and NULL passed as the handle in other functions.	This will
 * open the default file for the duration of the function and then close it. Where multiple
 * functions are called or if a non-default path is required, the policy file can be opened
 * explicitly and the SECPOL_USE_AS_DEFAULT flag passed in to indicate that the handle returned
 * should be used for calls where a NULL handle is used.
 *
 * In most cases, NULL should be passed for the path parameter and SECPOL_USE_AS_DEFAULT for the
 * flags to ensure a common security policy file is used by all processes and to avoid the file
 * being opened multiple times.
 *
 * If SECPOL_USE_AS_DEFAULT is used and previous call installed a default handle, the call
 * will fail with an errno of EBUSY. This can usually be ignored as there is already a policy
 * file accessible for other functions to use.
 *
 * @param[in] path	Path to the file.  If NULL the default path is used.
 * @param[in] flags Zero or more flags from secpol_open_flags_e ORred together.
 *
 * @return	A handle to the file.
 * @retval NULL Open failed.  The specific cause of the error is indicated by @c errno.
 */
secpol_file_t *secpol_open(const char *path, uint32_t flags);

/**
 * Close the security policy file.
 *
 * @param[in] handle  Handle to the security policy file.
 *
 * @retval None
 */
void secpol_close(secpol_file_t *handle);

/**
 * Find and possibly return a given blob.  This may either be used to find a blob of a particular type, typically
 * referenced by one of the SECPOL_BLOB_* constants.  It may also be used to iterate through all blobs in the file
 * with no prior knowledge of its contents by using SECPOL_BLOB_NEXT as the type.
 *
 * The possible errors are:
 * ENOENT - The specified blob does not exist
 * ENOMEM - Out of memory allocating blob
 * EIO    - I/O failure reading blob
 *
 * @param[in] handle		Handle to the security policy file.	 This may be NULL to allow use of a default
 *							file.
 * @param[in] type			Type of blob to search for or the constant SECPOL_BLOB_NEXT to return the next blob in
 *							the file.
 * @param[out] blob_info	If non-NULL, updated with information on blob
 * @param[out] data			If non-NULL, updated to contain a pointer to the contents of the blob. This
 *							memory must be freed by the caller.
 *
 * @retval 0  Blob was found
 * @retval -1 An error occurred.  The specific cause of the error is indicated by @c errno.
 */
int secpol_find_blob(secpol_file_t *handle, uint32_t type, secpol_blob_t *blob_info, void **data);

/**
 * Find and possibly return a given custom class blob.
 *
 * @param[in] handle		Handle to the security policy file.	 This may be NULL to allow use of a default
 *							file.
 * @param[in] name			The name of the class associated with the blob
 * @param[out] blob_info	If non-NULL, updated with information on blob
 * @param[out] data			If non-NULL, updated to contain a pointer to the contents of the blob. This
 *							memory must be freed by the caller.
 *
 * @retval 0  Blob was found
 * @retval -1 An error occurred.  The specific cause of the error is indicated by @c errno.
 */
int secpol_find_custom_blob(secpol_file_t *handle, const char *name, secpol_blob_t *blob_info, void **data);

/**
 * Reset the handle so that secpol_find_blob with SECPOL_BLOB_NEXT returns the first blob.
 *
 * @param[in] handle	 Handle to the security policy file.
 *
 * @retval None
 */
void secpol_reset(secpol_file_t *handle);

/**
 * Return the type id associated with a type name
 *
 * The possible errors are:
 * ESECTYPEINVAL - Unknown type
 *
 * @param[in] handle	 Handle to the security policy file.  This may be NULL to allow use of a default
 *						 file.
 * @param[in] type_name	 Name of type
 *
 * @return Type id associated with name
 * @retval SECPOL_INVALID_TYPE if name was not found
 */
uint32_t secpol_get_type_id(secpol_file_t *handle, const char *type_name);

/**
 * Return the type name associated with a type id
 *
 * The possible errors are:
 * EINVAL        - NULL handle provided and there is no default file opened
 * ESECTYPEINVAL - Unknown type
 *
 * @param[in] handle	 Handle to the security policy file.  This may be NULL to allow use of a
 *						 file previously opened with the SECPOL_USE_AS_DEFAULT flag.
 * @param[in] type_id	 Id of the type
 *
 * @return Name of the type.
 * @retval NULL if the type id was not found.
 */
const char *secpol_get_type_name(secpol_file_t *handle, uint32_t type_id);

/**
 * Flags for @c secpol_transition_type, @c secpol_posix_spawnattr_settypeid and @c secpol_resolve_name.
 */
enum secpol_flags_e {
	/** Name refers directly to a type name */
	SECPOL_TYPE_NAME = 1,
};

/**
 * Return the type id of the process.  Note that this is accurate only if the process does not call
 * @c procmgr_set_type_id and instead make any type changes by calling @c secpol_transition_type.
 *
 * @return Type id of the process
 */
uint32_t secpol_type_id(void);

/**
 * Resolve a name to an id.
 *
 * By default the name is assumed to refer to a derived type, that is a type that is selected based on
 * the process' current type and the name passed in. It is also possible to have name represent the
 * actual type name by passing SECPOL_TYPE_NAME in the flags.
 *
 * @param[in] handle	 Handle to the security policy file.  This may be NULL to allow use of a default
 *						 file.
 * @param[in] name		 Name to be looked up.
 * @param[in] flags		 Zero or more flags from secpol_flags_e ORred together.
 *
 * @return Type id that is associated with @c name based on the current type of the process.
 * @retval SECPOL_INVALID_TYPE if there is no type id to return.
 */
uint32_t secpol_resolve_name(secpol_file_t *handle, const char *name, uint32_t flags);


/**
 * Transition to a new type. Attempts to switch to a new type that is either specified as a type
 * name, or more commonly as a string, such as "run" that will be used to derive a type from the
 * current type of the process.
 *
 * If the function is successful, it indicates that there is a security policy in use, the process
 * is now running with whatever capabilities were deemed appropriate and that the process should not
 * itself attempt to modify abilities or switch uid as a means of dropping privilege.
 *
 * If the function fails, the process should instead perform whatever privilege dropping procedure
 * it supports, including nothing.	In the event of a failure in cases where a policy is in force,
 * system trace events will be emitted to allow the problem to be diagnosed.
 *
 * Behavior should usually be based only on success or failure, not on the specific cause of failure.
 * Failure of this function should not be considered fatal as it will render the program unable to
 * run without security policies.
 *
 * By default a derived type is used, that is the type id is selected based on the process' current type
 * and the name passed in. It is also possible to have name represent the actual type name by passing
 * SECPOL_TYPE_NAME in the flags.
 *
 * It is expected this function will be called only once following initialization.	 Multiple calls
 * to the function are unlikely to yield any security benefits.
 *
 * The possible errors are:
 * ENOTSUP - No security policy is in use
 * EPERM   - Permission denied
 * ESECTYPEINVAL - Unknown type or no derive rule for specified name
 *
 * @param[in] handle	 Handle to the security policy file.  This may be NULL to allow use of a default
 *						 file.
 * @param[in] name		 Name of type or name to derive type from based on current type.  If a derived
 *						 type is used, NULL may be passed to use the default name "run".
 * @param[in] flags		 Zero or more flags from secpol_flags_e ORred together.
 *
 * @retval 0  Type was successfully switched.
 * @retval -1  No type change was performed.
 */
int secpol_transition_type(secpol_file_t *handle, const char *name, uint32_t flags);

/**
 * Update a posix_spawn attribute object to spawn a child with a different type if appropriate.	 If a
 * security policy is in force, this function updates the posix spawn attribute structure with the type
 * to spawn as. If a policy is not in force, the function does nothing.
 *
 * By default a derived type is used, that is the type id is selected based on the process' current type
 * and the name passed in. It is also possible to have name represent the actual type name by passing
 * SECPOL_TYPE_NAME in the flags.
 *
 * If the process spawns multiple child processes that are expected to have different security needs,
 * a different name can be used for each to allow the security policy to be more closely tailored to
 * fit actual security requirements.  If the security of all children are likely substantially the same
 * NULL can be passed for the name to use the default name "child".	 This may only be done if a derived
 * type is used.
 *
 * The possible errors are:
 * EINVAL - NULL name specified with SECPOL_TYPE_NAME flag or the value specified by attrp was invalid
 * ESECTYPEINVAL - Unknown type or no derive rule for specified name
 *
 * @param[in] handle	 Handle to the security policy file.  This may be NULL to allow use of a default
 *						 file.
 * @param[in,out] attrp	 A pointer to the spawn attributes object that you want to update.
 * @param[in] name		 Name of type or name to derive type from based on current type.  If a derived
 *						 type is used, NULL may be passed to use the default name "child".
 * @param[in] flags		 Zero or more flags from secpol_flags_e ORred together.
 *
 * @retval 0  Spawn attributes were successfully updated or no update was wanted.
 * @retval -1  Attribute update failed or no valid type was found.
 */
int secpol_posix_spawnattr_settypeid(secpol_file_t *handle, posix_spawnattr_t *attrp, const char *name,
									 uint32_t flags);

typedef struct secpol_permission_s secpol_permission_t;

/**
 * Flags for @c secpol_get_permission
 */
enum secpol_get_permission_flags_e {
	/** If set, @c secpol_get_permission will fail and return NULL if the there is no policy
	 * in use or the specified class or permission does not exist.  If not set the permission
     * will be allowed for root users when there is no policy and never allowed when there is. */
	SECPOL_PERM_STRICT = 1,
};

/**
 * Returns a handle to a permission associated with a custom class.	 By default the function will
 * succeeed even when no security policy is in use or if the class or permission cannot be found however
 * the permission will always be denied.  This behavior may be changed by passing appropriate flags.
 *
 * The possible errors are:
 * ENOTSUP - No security policy is in use
 * ENOSYS  - Class or permission not in policy file or policy id wrong
 * EINVAL  - Invalid parameters to function
 * ENOMEM  - Out of memory
 * ENOENT etc as returned from a call to open the policy file.
 *
 * @param[in] handle	 Handle to the security policy file.  This may be NULL to allow use of a default
 *						 file.
 * @param[in] class		 Name of class associated with permission
 * @param[in] permission Name of permission
 * @param[in] flags		 Zero or more flags take from enum secpol_get_permission_flags_e
 *
 * @return A handle to the permission
 * @retval NULL The call failed. The specific cause of the error is indicated by @c errno.
 */
secpol_permission_t *secpol_get_permission(secpol_file_t *_handle, const char *_class, const char *_permission, unsigned flags);

/**
 * Test if a given type has a permission.
 *
 * The possible errors are:
 * EPERM   - Permission denied
 * EINVAL  - Invalid parameters to function
 * ENOMEM  - Out of memory
 *
 * Usually all errors should be treated identically
 *
 * @param[in] info		 The message info associated with the message that this check relates to.
 * @param[in] otype		 The type of the object being access or acted upon
 * @param[in] permission The name of the permission being tested
 *
 * @retval 0  The type ptype has the indicted permission for the type otype
 * @retval -1  The call failed. The specific cause of the error is indicated by @c errno.
 */
int secpol_check_permission(const struct _msg_info *info, uint32_t otype, const secpol_permission_t *permission);

/**
 * Locate an entry associated with a type id in an array that is sorted by type id and where the first element is
 * the type id.
 *
 * @param[in] type_id  Type id to search for.
 * @param[in] array	   Array to search
 * @param[in] ecount   Number of elements
 * @param[in] esize	   Size of each element in bytes
 *
 * @return A pointer to the element
 * @retval NULL	 An element wasn't found with the indicated type id.
 */
void *secpol_find_entry(uint32_t type_id, void *array, uint32_t ecount, uint32_t esize);

/**
 * Return the crc32 of block of data.
 *
 * @param[in] buffer	Pointer to a buffer of data
 * @param[in] length	Length of data
 *
 * @return The CRC
 */
uint32_t secpol_crc32(const void *buffer, uint32_t length);

/**
 * Return the policy id of any policy that has been pushed to procnto.	This should be used to verify that it
 * matches the policy id of any blob that is used from a policy file as mismatched policies might have unwanted
 * results.
 *
 * @param[out] policy_id	Address of a variable to receive the policy id.
 *
 * @retval 0  Call was successful
 * @retval -1 Request failed. The specific cause of the error is indicated by @c errno.
 */
int secpol_get_policy_id(uint32_t *policy_id);

/** Constant that can be passed to @c secpol_find_entry to return the next blob of any type. */
#define SECPOL_BLOB_NEXT 0xffffffff

#endif	// __QNX__


/** @cond */
__END_DECLS
/** @endcond */

#endif


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/secpol/public/secpol/secpol.h $ $Rev: 886230 $")
#endif
