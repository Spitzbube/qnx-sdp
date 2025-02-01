/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems. All Rights Reserved.
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
 * @param[in] handle	 Handle to the security policy file.  This may be NULL to allow use of a default
 *						 file.
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

/** TMAP - Blob for converting type names to and from type ids */
#define SECPOL_BLOB_TYPE_MAP 0x50414D54

/**
 * Initial data contained in a blob of type SECPOL_BLOB_TYPE_MAP.
 */
typedef struct {
	/** Blob type = SECPOL_BLOB_TYPE_MAP */
	uint32_t blob_type;
	/** Number of types */
	uint32_t type_count;
	/** Offset to an array of type_count uint32_t values containing the offset in the string table that contains the */
	/** name associated with a type id. */
	uint32_t id_to_name_offset;
	/** Offset to an array of type_count uint32_t containing type ids ordered by type name. */
	uint32_t sorted_id_offset;
	/** Offset in blob to string table, a packed array of type names. */
	uint32_t string_offset;
	/** Size of string table in bytes */
	uint32_t string_size;
} secpol_tm_blob_t;


/**
 * Blob type for the PMA1 blob, which defines security rules governing path manager attach operations, such as
 * resmgr_attach, name_attach and pathmgr_link.	 The data contained in this blob is of type secpol_pma_blob_t
 * with additional data following.
 */
#define SECPOL_BLOB_PMA_RULES 0x31414D50

/**
 * Initial data contained in a blob of type SECPOL_BLOB_PMA_RULES.
 */
typedef struct {
	/** Blob type = SECPOL_BLOB_PMA_RULES */
	uint32_t blob_type;
	/** Id that can be used to validate that blobs were generated from the same set of rules */
	uint32_t policy_id;
	/** Flags, not currently used, must be zero. */
	uint32_t flags;
	/** Offset in blob to an array secpol_pma_rule_list_t objects.	Each element provides information on paths
	 * that may be attached to by processes that are in a particular security domain.  The elements are ordered
	 * by increasing value of type id.
	 */
	uint32_t domains_offset;
	/** Number of elements in domains array */
	uint32_t domain_count;
	/**	 Offset in blob to array of rules.	Each element is of type secpol_pma_rule_t. */
	uint32_t rules_offset;
	/** Number of elements in rules array */
	uint32_t rule_count;
	/** Offset in blob to string table, a packed array of paths. */
	uint32_t paths_offset;
	/** Size of paths array in bytes */
	uint32_t paths_size;
	/** Must be zero */
	uint32_t zero[2];
} secpol_pma_blob_t;

/** Structure defining the offset and number of rules associated with a security domain. */
typedef struct {
	/** Type id associated with process trying to attach */
	uint32_t type_id;
	/** Index of the first rule for this process within the rules array. */
	uint32_t rule_index;
	/** Number of rules for the process */
	uint32_t rule_count;
} secpol_pma_rule_list_t;

/** Flags associated with a single pathmgr attach rule */
typedef enum {
	/** If set, the path associated with the rule should be matched with fnmatch */
	SECPOL_PMA_RULE_PATTERN = 1,
	/** If set, the path should match if it matches an initial directory portion of the path being attached. */
	SECPOL_PMA_RULE_LEADING_DIR = 2,
	/** If set a channel or a symlink may be attached to the path with no relabelling */
	SECPOL_PMA_RULE_LINK = 4,
} secpol_pma_rule_flag_t;

/** A single pathmgr attach rule. */
typedef struct {
	/** Offset relative to path string table of the path. */
	uint32_t path_offset;
	/** Zero or more flags from secpol_pma_rule_flag_t */
	uint32_t flags;
	/** The new type id to use for the channel (might be the same as the process type) */
	uint32_t new_type;
} secpol_pma_rule_t;

/** Blob type for the ABLE blob, which defines procmgr abilities that should be given to processes based on the
 * type id of the security domain they are associated with.	 These procmgr abilities might need to be combined with
 * others not derived from the security policy rules.
 */
#define SECPOL_BLOB_ABILITIES  0x454C4241

#define SECPOL_DYNAMIC_ABILITY_OFFSET 0x8000

/** Flags associated with the SECPOL_BLOB_ABILITIES blob */
typedef enum {
    /** If set, the dyn_perm_offset field in the abilities blob is valid. */
	SECPOL_ABILITY_HAS_PERMISSIONS = 1,
} secpol_ability_blob_flag_t;

typedef enum {
    /** Indicates permissions for dynamic ability should be set when policy is pushed. */
    SECPOL_ABILITY_PERM_VALID = 1,
    /** Indicates root should be given the ability by default. */
    SECPOL_ABILITY_PERM_ALLOW_ROOT = 2,
    /** Indicates non-root should be given the ability by default. */
    SECPOL_ABILITY_PERM_ALLOW_NONROOT = 4,
} secpol_ability_perms_t;

/**
 * Initial data contained in a blob of type SECPOL_BLOB_ABILITIES.
 */
typedef struct {
	/** Blob type = SECPOL_BLOB_ABILITIES */
	uint32_t blob_type;
	/** Id that can be used to validate that blobs were generated from the same set of rules */
	uint32_t policy_id;
	/** Flags, not currently used, must be zero. */
	uint32_t flags;
	/** Offset in blob to an array secpol_ability_type_t objects.  Each element provides information on abilities
	 * that should be provided to a process with the given type.  The array is ordered by type_id.
	 */
	uint32_t types_offset;
	/** Number of elements in domains array */
	uint32_t type_count;
	/**	 Offset in blob to array of uint32_t items representing abilities. Ability numbers in excess of
	 * SECPOL_DYNAMIC_ABILITY_OFFSET represent dynamic abilities.
	 */
	uint32_t ability_offset;
	/** Size of ability array in bytes. */
	uint32_t ability_size;
	/** Offset to an array of uint32_t items representing the offsets in the dynamic ability name array of dynamic
	 *	abilities.
	 */
	uint32_t dyn_name_idx_offset;
	uint32_t dyn_name_idx_count;
	uint32_t dyn_names_offset;
	uint32_t dyn_names_size;
    /** Offset to an array of dyn_name_idx_count bytes that define the permissions for dynamic abilities.
        Each byte contains zero or more values from secpol_ability_perms_t. */
    uint32_t dyn_perm_offset;
} secpol_ability_blob_t;

/** Information on procmgr abilities to be assigned to a type to support enforcement of the rules contained in
 * the security policy.
 */
typedef struct {
	/** Type id associated with security domain */
	uint32_t type_id;
	/** Offset to first ability entry in the ability array for this security domain */
	uint32_t ability_offset;
	/** Size in bytes of ability entries for this security domain */
	uint32_t ability_size;
	/** Number of ability entries for this security domain */
	uint32_t ability_count;
	/** crc32 computed over ability entries */
	uint32_t crc;
} secpol_ability_type_t;

/** Blob type for the QNET blob, which defines type ids of channels that a process may connect to over QNET.
 */
#define SECPOL_BLOB_QNET  0x54454E51

/**
 * Initial data contained in a blob of type SECPOL_BLOB_QNET.
 */
typedef struct {
	/** Blob type = SECPOL_BLOB_QNET */
	uint32_t blob_type;
	/** Id that can be used to validate that blobs were generated from the same set of rules */
	uint32_t policy_id;
	/** Flags, not currently used, must be zero. */
	uint32_t flags;
	/** Offset in blob to an array of secpol_qnet_type_t objects.  Each element provides information on channel
	 * type ids that may be connected to.  The array is ordered by type_id.
	 */
	uint32_t types_offset;
	/** Number of elements in array */
	uint32_t type_count;
	/** Offset in blob to array of uint32_t items representing channel type ids. */
	uint32_t channel_type_offset;
	/** Number of elements in array. */
	uint32_t channel_type_count;
} secpol_qnet_blob_t;

/** Information on type ids of channels that a process of a single type id may connect to over QNET.
 */
typedef struct {
	/** Type id associated with security domain */
	uint32_t type_id;
	/** Index of the first channel type id entry in the array for this security domain */
	uint32_t channel_type_index;
	/** Number of channel type id entries for this security domain */
	uint32_t channel_type_count;
} secpol_qnet_type_t;

/** Blob type for the DRVT blob, which defines a mapping between types and other types based on
  a string.
 */
#define SECPOL_BLOB_DERIVE	0x54565244

/**
 * Initial data contained in a blob of type SECPOL_BLOB_DERIVE.
 */
typedef struct {
	/** Blob type = SECPOL_BLOB_DERIVE */
	uint32_t blob_type;
	/** Id that can be used to validate that blobs were generated from the same set of rules */
	uint32_t policy_id;
	/** Offset in blob to an array of secpol_derive_type_t objects.	 Each element provides the
	 * location and size of the mapping information for the type. The array is ordered by type_id.
	 */
	uint32_t types_offset;
	/** Number of elements in types array */
	uint32_t type_count;
	/** Offset in blob to an array of secpol_derive_mapping_t objects defining a mapping between
		a string and a type id.
	*/
	uint32_t mapping_offset;
	/** Number of elements in mapping array */
	uint32_t mapping_count;
	/** Offset in blob to string table, a packed array of names used for mapping types to
		other types.
	*/
	uint32_t string_offset;
	/** Size of string table in bytes */
	uint32_t string_size;
} secpol_derive_blob_t;

typedef struct {
	/** Type id associated with security domain */
	uint32_t type_id;
	/** Index of first mapping for the type. */
	uint32_t map_offset;
	/** Number of mappings for the type. */
	uint32_t map_count;
} secpol_derive_type_t;

typedef struct {
	/** Offset into string table */
	uint32_t str_offset;
	/** Type id associated with the string */
	uint32_t derived_type_id;
} secpol_derive_mapping_t;

/** Blob type for the CIDX blob, which provides an index for custom rule blobs (of type
 *	CUST).
 */
#define SECPOL_BLOB_CUSTOM_INDEX  0x58444943

typedef struct {
	/** Blob type = SECPOL_BLOB_CUSTOM_INDEX */
	uint32_t blob_type;
	/** Id that can be used to validate that blobs were generated from the same set of rules */
	uint32_t policy_id;
	/** Offset in blob of array of secpol_custom_index_entry_t objects. */
	uint32_t custom_index_offset;
	/** Number of objects in the custom index array */
	uint32_t custom_index_count;
	/** Offset in blob to string table, a packed array of object and permission names. */
	uint32_t string_offset;
	/** Size of string table in bytes */
	uint32_t string_size;
} secpol_custom_index_blob_t;

typedef struct {
	/** Offset in string table of custom class name. */
	uint32_t name_offset;
	/** Offset in file of blob containing the rules for the class. */
	uint32_t blob_offset;
	/** Size of blob */
	uint32_t blob_size;
} secpol_custom_index_entry_t;

/** Blob type for the CUST blob, which defines the rules for a custom type
  a string.
 */
#define SECPOL_BLOB_CUSTOM	0x54535543

typedef struct {
	/** Blob type = SECPOL_BLOB_CUSTOM */
	uint32_t blob_type;
	/** Id that can be used to validate that blobs were generated from the same set of rules */
	uint32_t policy_id;
	/** Integer id representing the class */
	uint32_t class_id;
	/** Offset into the blob to an array of secpol_custom_permission_t objects defining the
		permissions associated with the object.
	*/
	uint32_t permission_offset;
	/** Number of elements in the permissions array */
	uint32_t permission_count;
	/** Offset in blob to an array of secpol_custom_type_t objects.	 Each element provides the
	 * location and size of the information for the type. The array is ordered by type_id.
	 */
	uint32_t types_offset;
	/** Number of elements in types array */
	uint32_t type_count;
	uint32_t range_offset;
	uint32_t range_count;
	/** Offset in blob to string table, a packed array of class and permission names.
	 *	The class name is at the beginning of the string table.
	 */
	uint32_t string_offset;
	/** Size of string table in bytes */
	uint32_t string_size;
} secpol_custom_class_blob_t;

/** Structure defining a permission associated with an object.
*/
typedef struct {
	/** Offset of name in string table */
	uint32_t name_offset;
	/** The index of the permission in permissions array of secpol_custom_type_t
	 * structures.
	 */
	uint32_t  index;
} secpol_custom_permission_t;

/** Structure representing a range of types that have a permission.	 The range is from low to high
	inclusive.
*/
typedef struct {
	uint16_t low;
	uint16_t high;
} secpol_custom_range_t;

typedef struct {
	// Range offset relative to the first offset for the type
	uint16_t offset;
	uint16_t count;
} secpol_custom_rule_t;

typedef struct {
	/** Type id associated with security domain */
	uint32_t type_id;
	/** Index of first rule for the type. */
	uint32_t range_offset;
	/** Array of permission information for the type.  The size of the array is equal to
	 *	the number of permissions associated with the object class.
	*/
	secpol_custom_rule_t permissions[0];
} secpol_custom_type_t;


/** @cond */
__END_DECLS
/** @endcond */

#endif


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/BC700_6762_sdp704/lib/secpol/public/secpol/secpol.h $ $Rev: 868809 $")
#endif
