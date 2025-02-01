/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. All Rights Reserved.
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

#if !defined(ADO_DICT_H_INCLUDED)
#define ADO_DICT_H_INCLUDED

#if !defined(__TYPES_H_INCLUDED)
#include <sys/types.h>
#endif

/*
  Dictionary object API

  A dictionary object is a collection of key+value pairs,
  where both the key and the value are strings.
 */

__BEGIN_DECLS

/**
    Dictionary handle used to keep track of dictionary objects.

    As a convention, function parameters and return values
    declared as pointers to non-const ado_dict_t
    indicate that the ownership of the handle
    (i.e. the responsibility for destroying it)
    is transferred to or from the function,
    whereas pointers to const char*
    indicate that no transfer of ownership occurs.
    When a function returns a pointer to a const ado_dict_t,
    the guaranteed lifetime of the returned handle
    needs to be documented explicitly.
 */
typedef struct ado_dict ado_dict_t;

/**
 *  Add or modify a dictionary entry.
 *  Creates a new dictionary object that is an exact replica of the old object,
 *  except the specified entry is added or modified.
 *  Returns a new handle for the new dictionary object, or NULL on failure.
 *  On success, the original dictionary handle is consumed.
 */
ado_dict_t* ado_dict_key_set( ado_dict_t *dict, const char *key, const char *value );

/**
 *  Return the value of the named entry (or a null pointer if not found).
 */
const char* ado_dict_key_lookup( const ado_dict_t *dict, const char *key );

/**
 *  Dump dictionary for debugging
 */
void ado_dict_dump( const ado_dict_t *dict );

/**
 *  Load the specified dictionary keys into the values array
 *  Args:
 *       const ado_dict_t* dict      -> Pointer to the card dictionary
 *       const char**      keys      -> Array of dictionary keys to lookup
 *       const char**      values    -> Array of values corresponding to the provided keys
 *       int               nOptions  -> Number of keys/values (size of keys and values arrays)
 *       int               nRequired -> Number of mandatory keys (0 if all keys are optional)
 *                                      Mandatory keys must be contiguous starting at array index 0
 *       int               index     -> Key index, an indexed key has an '_<index>' appended to the key name (i.e key_1)
 *                                      Non-indexed key values will apply globally for the section it is defined under.
 *                                      This value can be overridden on a device by device basis with indexed keys (-1 to disable index override)
 *  Returns:
 *        0 on succcess
 *       -1 on failure (Mandatory key missing)
 *
 */
int ado_config_load_key_values(const ado_dict_t* dict, const char** keys, const char** values, int nOptions, int nRequired, int index);

/**
 *  Apply the specified dictionary keys values back to the dictionary
 */
ado_dict_t *ado_config_set_key_values(ado_dict_t* dict, const char** keys, const char** values, int nOptions);

__END_DECLS

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/BC700_6762_sdp704/services/audio/public/include/ado_dict.h $ $Rev: 838346 $")
#endif
