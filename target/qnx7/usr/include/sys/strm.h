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

#if !defined(SYS_STRM_H_INCLUDED)
#define SYS_STRM_H_INCLUDED

#if !defined(__TYPES_H_INCLUDED)
#include <sys/types.h>
#endif

__BEGIN_DECLS


/**
 *  @file strm.h
 *
 *  @brief Dictionary Object API
 *
 *  A dictionary object is a collection of key-value pairs. Each pair is
 *  considered an entry or a <em>named value</em>, where the key acts as the
 *  entry's name. Keys are unique, so you can't add an entry with a duplicate
 *  key.
 *
 *  The Dictionary Object API allows you to specify keys and values as
 *  C-strings or as shareable strings. Using C-strings is convenient but using
 *  shareable stings may be more efficient, especially if you use the same keys
 *  and values in multiple program components.
 *
 *  A <em>shareable string</em> is a data structure that stores a string and
 *  keeps track of how many handles to it exist. This design allows you to
 *  clone shareable string handles and have separate components take ownership
 *  of individual handles so they can use the string independently of each
 *  other, without copying all of the string characters.
 *  It also allows the @c libstrm library to safely destroy the string
 *  when the last handle is destroyed, without impacting other components.
 *  Note that in dictionaries, both the keys and values are stored in
 *  shareable strings.
 *
 *  The API allows you to create multiple handles to a dictionary object and
 *  then use and even delete these handles in independent components.
 *  After it's created, a dictionary is immutable until it's destroyed, so
 *  separate components can access it through their own handles and read the
 *  exact same entries, without worrying that the dictionary could change
 *  between reads.
 */


/**
 *  @brief Dictionary object
 *
 *  The @c strm_dict_t structure is a private data type representing a
 *  dictionary object. Pointers to this data type are used as dictionary
 *  handles in API functions.
 *
 *  To create a new dictionary handle, call strm_dict_new(). The handle lets
 *  you access the dictionary content by reading, adding, modifying, or
 *  removing entries.
 *
 *  For functions that update dictionary entries, the original dictionary
 *  handle is destroyed and a new dictionary is created and a handle to it
 *  returned. The contents of the new dictionary are identical to the previous 
 *  one except for the entry that was updated. This design means that any 
 *  program components with handles to the previous dictionary will still see 
 *  the older set of entries.
 *
 *  You can clone handles with strm_dict_clone(); this is more efficient than
 *  copying all of the dictionary content. You must call strm_dict_destroy()
 *  separately for each cloned handle to properly dispose of any resources
 *  associated with it.
 *
 *  Only when the last handle to a dictionary gets destroyed does the library
 *  free the object's memory (because it's now safe to do so).
 */
typedef struct strm_dict strm_dict_t;

/**
 *  @brief Create a new dictionary handle to an empty dictionary
 *
 *  This function creates a new dictionary handle to an empty dictionary. 
 *  The handle is a pointer to a @c strm_dict_t object and can be passed into 
 *  subsequent API calls to read or modify the dictionary content.
 *
 *  @return A new dictionary handle.
 */
strm_dict_t *strm_dict_new( void );

/**
 *  @brief Clone a dictionary handle
 *
 *  This function clones the dictionary handle in @e dict. Here, cloning means
 *  creating a new handle that points to identical dictionary content.
 * 
 *  With this design, each program component can use its own clone of a
 *  dictionary handle and destroy the handle when it's done using the
 *  dictionary, without impacting other components.
 *
 *  The new handle may represent the same pointer value as the original handle
 *  and hence, be considered equal by the comparison operator. But there's an
 *  important difference between cloning a handle using strm_dict_clone()
 *  versus making copies of it by pointer assignment or memcpy():
 *  Because a dictionary keeps track of how many handles to it have been
 *  created, each of those handles must be destroyed in its own
 *  strm_dict_destroy() call. Conversely, if you have made multiple copies of
 *  a single handle, you can use any one of them to destroy it, and you should
 *  not use any of them afterwards.
 *
 *  @param dict An existing dictionary handle.
 *  @return A new handle to the dictionary object on success, or a null pointer
 *          on failure.
 */
strm_dict_t *strm_dict_clone( const strm_dict_t *dict );

/**
 *  @brief Destroy a dictionary handle
 *
 *  This function destroys the specified dictionary handle and if this is the
 *  last handle, frees the memory allocated for the dictionary object.
 *
 *  @param dict A dictionary handle.
 *
 *  @return Zero on success, -1 on failure (check @e errno).
 */
int strm_dict_destroy( strm_dict_t *dict );

/**
 *  @brief Add or modify a dictionary entry using strings
 *
 *  This function creates a new dictionary that is an exact replica of
 *  the one specified by @e dict, except that the entry specified by @e key
 *  is added or modified. The entry's value is set to @e value.
 *
 *  When the function succeeds, the original dictionary handle is destroyed and
 *  a handle to the new dictionary is returned.
 *
 *  If the original handle is the only one for that dictionary, the library can
 *  reuse the memory of the original dictionary in creating the new one.
 *  This means that if you build a dictionary by repeatedly calling
 *  strm_dict_set() and passing in the handle returned by the previous call,
 *  the library just keeps modifying the dictionary without having to make
 *  copies of the existing keys and values.
 *
 *  This same reusing of memory when there's only one handle to the dictionary
 *  is done by other functions that modify individual entries, such as
 *  strm_dict_set_index().
 *
 *  @param dict  A dictionary handle.
 *  @param key   The key of the entry to add or modify.
 *  @param value The new value of the entry.
 *
 *  @return A handle to the new dictionary on success, or a null pointer
 *          on failure.
 */
strm_dict_t *strm_dict_set( strm_dict_t *dict, const char *key, const char *value );

/**
 *  @brief Modify a dictionary entry by index, using a string
 *
 *  This function creates a new dictionary that is an exact replica of the one
 *  specified by @e dict, except that the value of the entry specified by
 *  @e index is replaced with @e value.
 *
 *  When the function succeeds, the original dictionary handle is destroyed and
 *  a handle to the new dictionary is returned.
 *
 *  @param dict  A dictionary handle.
 *  @param index The index of the entry to modify. The index is zero-based.
 *  @param value The new value of the entry.
 *
 *  @return A handle to the new dictionary on success, or a null pointer
 *          on failure (if you provided an out-of-range index, @e errno is set
 *          to ESRCH).
 */
strm_dict_t *strm_dict_set_index( strm_dict_t *dict, size_t index, const char *value );

/**
 *  @brief Delete a dictionary entry by key
 *
 *  This function creates a new dictionary that is an exact replica of the one
 *  specified by @e dict, except that the entry specified by @e key is deleted.
 *
 *  If no such entry is found, the original dictionary is left unmodified and
 *  the same handle is returned. If an entry is found, the original dictionary
 *  handle is destroyed and a handle to the new dictionary is returned.
 *
 *  @param dict  A dictionary handle.
 *  @param key   The key of the entry to delete.
 *
 *  @return On success, a handle to the new dictionary (when the entry was
 *          deleted) or to the existing dictionary (when the entry wasn't
 *          found). On failure, a null pointer.
 */
strm_dict_t *strm_dict_key_delete( strm_dict_t *dict, char const *key );

/**
 *  @brief Delete a dictionary entry by index
 *
 *  This function creates a new dictionary that is an exact replica of
 *  the one specified by @e dict, except that the entry specified by @e index
 *  is deleted.
 *
 *  When the function succeeds, the original dictionary handle is destroyed and
 *  a handle to the new dictionary is returned.
 *
 *  @param dict   A dictionary handle.
 *  @param index  The index of the entry to delete. This index is zero-based.
 *
 *  @return A handle to the new dictionary on success, or a null pointer
 *          on failure (if you provided an out-of-range index, @e errno is
 *          set to ESRCH).
 */
strm_dict_t *strm_dict_index_delete( strm_dict_t *dict, size_t index );

/**
 *  @brief Get the value of a dictionary entry as a string, based on its key
 *
 *  This function returns the value of the entry specified by @e key and found
 *  in the @e dict dictionary. The returned string is owned by the dictionary
 *  and remains valid until the dictionary handle is destroyed.
 *
 *  @param dict  A dictionary handle.
 *  @param key   The key of the entry whose value is being retrieved.
 *
 *  @return If the entry is found, its value (as a string) is returned.
 *          If it's not found, a null pointer is returned.
 */
const char *strm_dict_find_value( const strm_dict_t *dict, const char *key );

/**
 *  @brief Get the index of a dictionary entry based on its key
 *
 *  This function returns the index of the entry specified by @e key and found
 *  in the @e dict dictionary. The index is zero-based.
 *
 *  @param dict  A dictionary handle.
 *  @param key   The key of the entry whose index is being retrieved.
 *
 *  @return If the entry is found, its zero-based index is returned.
 *          If it's not found, -1 is returned.
 */
ssize_t strm_dict_find_index( const strm_dict_t *dict, const char *key );

/**
 *  @brief Get the number of entries in a dictionary
 *
 *  This function returns the number of entries in the dictionary specified by
 *  @e dict.
 *
 *  @param dict  A dictionary handle.
 *
 *  @return The number of entries in the dictionary.
 */
size_t strm_dict_size( const strm_dict_t *dict );

/**
 *  @brief Get the key of a dictionary entry as a string, based on its index
 *
 *  This function finds the key of the entry stored at the zero-based index
 *  given in @e n, in the dictionary specified by @e dict. It returns the key
 *  as a string. This string is owned by the dictionary and remains valid
 *  until the dictionary handle is destroyed.
 *
 *  @param dict  A dictionary handle.
 *  @param n     The zero-based index of the entry whose key is being
 *               retrieved.
 *
 *  @return The specified key as a string on success, or a null pointer
 *          on failure.
 */
const char *strm_dict_key_get( const strm_dict_t *dict, size_t n );

/**
 *  @brief Get the value of a dictionary entry as a string, based on its index
 *
 *  This function finds the value of the entry stored at the zero-based index
 *  given in @e n, in the dictionary specified by @e dict. It returns the value
 *  as a string. This string is owned by the dictionary and remains valid until
 *  the dictionary handle is destroyed.
 *
 *  @param dict  A dictionary handle.
 *  @param n     The zero-based index of the entry whose value is being
 *               retrieved.
 *
 *  @return The specified value as a string on success, or a null pointer
 *          on failure.
 */
const char *strm_dict_value_get( const strm_dict_t *dict, size_t n );


/*
  Shareable string handle API--the "advanced" verion of the Dictionary API.

  This API portion allows the creation of multiple handles to a string.
  They can be used and destroyed by independent program components.
  Note that multiple handles may be represented by identical pointers;
  you should not compare handles.
  Since the string represented by a handle is immutable until destroyed,
  different components can access it through their individual handles
  and even destroy their handles without worrying about synchronization.
*/

/**
 *  @brief Shareable string object
 *
 *  The @c strm_string_t structure is a private data type representing a
 *  shareable string object. Pointers to this data type are used as handles to
 *  shareable strings in API functions.
 *
 *  You can create a new shareable string containing a specific C-string
 *  by calling strm_string_make(). This function returns a shareable string
 *  handle that can you use to define a key or value when building a
 *  dictionary. You can also obtain a pointer to the first character in the
 *  string, create a new shareable string, or destroy the shareable string.
 *
 *  Different shareable string handles may be represented by identical
 *  pointers; you should not compare handles. Regardless of how handles are
 *  represented internally, you must call strm_string_destroy() separately
 *  for each handle that you own to properly dispose of any resources
 *  associated with it.
 *  Some API calls take ownership of (consume) a shareable string handle passed
 *  in as an argument. Their function descriptions state when they do this.
 *  In these cases, you shouldn't destroy the handle; the library will do so
 *  when appropriate.
 *
 *  Only when the last handle to a shareable string gets destroyed does the
 *  library free the object's memory (because it's now safe to do so).
 */
typedef struct strm_string strm_string_t;

/**
 *  @brief Create a new shareable string containing a copy of the provided
 *  string
 *
 *  This function creates a new shareable string, populating it with the string
 *  passed in @e cstring, and returns a handle to the new object.
 *
 *  @param cstring  A pointer to a null-terminated string.
 *
 *  @return A handle to the new shareable string.
 */
strm_string_t *strm_string_make( const char *cstring );

/**
 *  @brief Allocate a new shareable string
 *
 *  This function allocates a new shareable string that can store @e len bytes,
 *  and stores a handle to this object in the variable pointed to by @e handle.
 *
 *  The caller must then write a null-terminated string into the buffer of the
 *  new shareable string before calling any of strm_string_clone(),
 *  strm_string_modify(), or strm_string_destroy(), and must not modify the
 *  buffer afterwards.
 *
 *  @param len     The size, in bytes, to allocate for the string buffer, not
 *                 including the null-terminator. This is the maximum length of
 *                 the string that you can store.
 *  @param handle  A pointer to a variable for storing the new string handle.
 *
 *  @return On success, a pointer to the first byte of the string buffer in the
 *          new shareable string. On failure, a null pointer.
 */
char *strm_string_alloc( size_t len, strm_string_t **handle );

/**
 *  @brief Return a pointer to the string in a shareable string
 *
 *  This function returns a pointer to the string in the shareable string
 *  referenced by @e sstr.
 *
 *  @param sstr  A handle to a shareable string.
 *
 *  @return A pointer to the first character of the string in the specified
 *          shareable string.
 */
const char *strm_string_get( const strm_string_t *sstr );

/**
 *  @brief Clone a shareable string handle
 *
 *  This function clones the shareable string handle in @e sstr.
 *  Here, cloning means creating a new handle that points to identical string
 *  content.
 * 
 *  With this design, each program component can use its own clone of a
 *  shareable string handle and destroy the handle when it's done using the
 *  string, without impacting other components.
 *
 *  The new handle may represent the same pointer value as the original handle
 *  and hence, be considered equal by the comparison operator. But there's an
 *  important difference between cloning a handle using strm_string_clone()
 *  versus making copies of it by pointer assignment or memcpy():
 *  Because a shareable string keeps track of how many handles to it have been
 *  created, each of those handles must be destroyed in its own
 *  strm_string_destroy() call. Conversely, if you have made multiple copies of
 *  a single handle, you can use any one of them to destroy it, and you should
 *  not use any of them afterwards.
 *
 *  @param sstr  An existing shareable string handle.
 *
 *  @return A new handle to the shareable string on success, or a null pointer
 *          on failure (@e errno is set).
 */
strm_string_t *strm_string_clone( const strm_string_t *sstr );

/**
 *  @brief Destroy a shareable string handle
 *
 *  This function destroys the shareable string handle referenced by @e sstr.
 *  If this is the last handle referencing the shareable string, the function
 *  frees the object's memory.
 *
 *  @param sstr  A handle to a shareable string.
 *
 *  @return Zero on success, -1 on failure (check @e errno).
 */
int strm_string_destroy( strm_string_t *sstr );

/**
 *  @brief Modify the string stored in a shareable string
 *
 *  This function modifies the string stored in a shareable string, 
 *  by destroying the original object referenced by @e sstr and creating a new
 *  object containing the string in @e cstring. 
 *
 *  Calling this function is equivalent to calling strm_string_destroy() and 
 *  strm_string_make() except that it may reuse the memory of the original 
 *  object. Note that the passed-in shareable string handle is consumed, even
 *  on failure.
 *
 *  @param sstr     A handle to an existing shareable string.
 *  @param cstring  A pointer to a null-terminated string to store in the new
 *                  shareable string.
 *
 *  @return On success, a handle to the new shareable string.
 *          On failure, a null pointer (@e errno is set).
 */
strm_string_t *strm_string_modify( strm_string_t *sstr, const char *cstring );


/*
   The "advanced" version of the Dictionary API.
   It uses handles to shareable strings instead of pointers to regular C-strings.
 */

/**
 *  @brief Add or modify a dictionary entry using shareable strings
 *
 *  This function creates a new dictionary that is an exact replica of
 *  the one specified by @e dict, except that the entry specified by @e key is
 *  added or modified. The entry's value is set to @e value.
 *
 *  The shareable string handles in the @e key and @e value arguments are
 *  consumed by the call, meaning you should not destroy them later because the
 *  dictionary will do so at the appropriate time. The original dictionary
 *  handle is destroyed on success but preserved on failure.
 *
 *  This function is equivalent to strm_dict_set(), except that it may be more
 *  efficient, especially if you use clones of the same @e key or @e value
 *  repeatedly.
 *
 *  @param dict   A dictionary handle.
 *  @param key    The key of the dictionary entry to add or modify.
 *  @param value  The new value for the dictionary entry.
 *
 *  @return On success, a handle to the new dictionary.
 *          On failure, a null pointer (if you provided an out-of-range index,
 *          @e errno is set to ESRCH).
 */
strm_dict_t *strm_dict_set_rstr( strm_dict_t *dict, strm_string_t *key, strm_string_t *value );

/**
 *  @brief Modify a dictionary entry by index, using a shareable string
 *
 *  This function creates a new dictionary that is an exact replica of
 *  the one specified by @e dict, except that the value of the entry specified
 *  by @e index is replaced with @e value.
 *
 *  The handle in the @e value argument is consumed by the call, meaning you
 *  should not destroy it later because the dictionary will do so at the
 *  appropriate time. The original dictionary handle is destroyed on success
 *  but preserved on failure.
 *
 *  This function is equivalent to strm_dict_set_index(), except that it may be
 *  more efficient, especially if you use clones of the same @e value
 *  repeatedly.
 *
 *  @param dict   A dictionary handle.
 *  @param index  The index of the dictionary entry to modify.
 *  @param value  The new value for the dictionary entry.
 *
 *  @return On success, a handle to the new dictionary.
 *          On failure, a null pointer (if you provided an out-of-range index,
 *          @e errno is set to ESRCH).
 */
strm_dict_t *strm_dict_set_index_rstr( strm_dict_t *dict, size_t index, strm_string_t *value );

/**
 *  @brief Get the value of a dictionary entry as a shareable string, based on
 *  its key
 *
 *  This function finds the dictionary entry specified by the string in @e key
 *  and returns a handle to the shareable string containing the entry's value.
 *
 *  The returned handle is owned by the dictionary and remains valid until the
 *  dictionary handle is destroyed. You can clone the shareable string handle
 *  to have one that exists independently of the dictionary.
 *
 *  @param dict   A dictionary handle.
 *  @param key    The key for the entry whose value is being retrieved.
 *
 *  @return If the entry is found, a handle to the shareable string containing
 *          its value. If it's not found, a null pointer.
 */
const strm_string_t *strm_dict_find_rstr( const strm_dict_t *dict, const char *key );

/**
 *  @brief Get the key of a dictionary entry as a shareable string, based on
 *  its index
 *
 *  This function finds the key of the entry stored at the zero-based index
 *  given in @e n, in the dictionary specified by @e dict. It returns a handle
 *  to the shareable string containing the entry's key.
 *
 *  The returned handle is owned by the dictionary and remains valid until the
 *  dictionary handle is destroyed. You can clone the shareable string handle
 *  to have one that exists independently of the dictionary.
 *
 *  @param dict  A dictionary handle.
 *  @param n     The zero-based index of the entry whose key is being
 *               retrieved.
 *
 *  @return If the entry is found, a handle to the shareable string containing
 *          its key. If it's not found, a null pointer.
 */
const strm_string_t *strm_dict_key_rstr( const strm_dict_t *dict, size_t n );

/**
 *  @brief Get the value of a dictionary entry as a shareable string, based on
 *  its index
 *
 *  This function finds the value of the entry stored at the zero-based index
 *  given in @e n, in the dictionary specified by @e dict. It returns a handle
 *  to the shareable string containing the entry's value.
 *
 *  The returned handle is owned by the dictionary and remains valid until the
 *  dictionary handle is destroyed. You can clone the shareable string handle
 *  to have one that exists independently of the dictionary.
 *
 *  @param dict  A dictionary handle.
 *  @param n     The zero-based index of the entry whose value is being
 *               retrieved.
 *
 *  @return If the entry is found, a handle to the shareable string containing
 *          its value. If it's not found, a null pointer.
 */
const strm_string_t *strm_dict_value_rstr( const strm_dict_t *dict, size_t n );

/**
 *  @brief Subtract one dictionary from another
 *
 *  This function creates a replica of the dictionary specified by @e left but
 *  with all entries that have matching keys in the one specified by @c right
 *  removed, regardless of their values.
 *
 *  Note that the function destroys the @e left handle, even on failure.
 *  If the same handle is passed in for both arguments, the function destroys
 *  it and returns a new handle to an empty dictionary. Otherwise, the @e right
 *  handle is preserved.
 *
 *  @param left   A handle to the first dictionary, which is the basis for the
 *                new dictionary.
 *  @param right  A handle to the second dictionary, which contains the keys
 *                that you want to remove.
 *
 *  @return A new handle to the resulting dictionary on success, or a null
 *          pointer on failure.
 */
strm_dict_t *strm_dict_subtract( strm_dict_t *left, strm_dict_t const *right );

/**
 *  @brief Compare two dictionaries to find the added and modified entries
 *
 *  This function compares two dictionaries to find the added and modified
 *  entries. It creates a replica of the one specified by @e newdict but
 *  removes all entries that have matching keys and values in @e olddict.
 *  In other words, if @e olddict is the older version of some dictionary and
 *  @e newdict is the newer version, the resulting dictionary contains all
 *  entries that were added or modified, but not the ones that were left alone
 *  or deleted.
 *
 *  Note that the function destroys the @e newdict handle, even on failure.
 *  If the same handle is passed in for both arguments, the function destroys
 *  it and returns a new handle to an empty dictionary. Otherwise, the
 *  @e olddict handle is preserved.
 *
 *  @param newdict  A handle to the newer version of a dictionary.
 *  @param olddict  A handle to the older version of a dictionary.
 *
 *  @return On success, a handle to the dictionary containing the result of the
 *          comparison. On failure, a null pointer.
 */
strm_dict_t *strm_dict_compare( strm_dict_t *newdict, strm_dict_t const *olddict );


__END_DECLS

#endif





#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/strm/public/sys/strm.h $ $Rev: 832469 $")
#endif
