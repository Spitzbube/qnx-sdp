/*
 * $QNXLicenseC:
 * Copyright 2017-2019, QNX Software Systems.  All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software.  Free development
 * licenses are available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#ifndef _QH_STRING_H_INCLUDED
#define _QH_STRING_H_INCLUDED

/**
 * @file string.h
 * QNX Helpers - String related helpers.
 */

/* sorted headers from this project */
#ifndef _QH_ATTR_H_INCLUDED
#include "qh/attr.h"
#endif

/* sorted library headers */

/* sorted system headers */
#ifndef _STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef __TYPES_H_INCLUDED
#include <sys/types.h>
#endif

/** @cond */
__BEGIN_DECLS
/** @endcond */

/**
 * Macro that makes a string from the passed-in argument name.
 */
#define QH_STR_STRINGIFY(__ARG) #__ARG

/**
 * Macro that makes a string from the passed-in argument's value.
 */
#define QH_STR_EXPANDED_STRINGIFY(__ARG) QH_STR_STRINGIFY(__ARG)

/**
 * Print formatted output into a newly allocated string.
 *
 * @param[in] fmt A string that specifies the format of the output. The formatting string determines
 *                what additional arguments you need to provide. For more information, see printf().
 *
 * @return A pointer to the newly allocated and formatted string, or NULL if an error occurred, in
 *         which case errno is set.
 */
char * qh_malloc_sprintf(const char *fmt, ...) QH_ATTR_FUNC_FORMAT_PRINTF(1, 2);

/**
 * Return a specified string, a fallback string if the specified string is NULL, or an empty string
 * if the fallback string is also NULL.
 *
 * Always returns a non-NULL string.
 *
 * @param[in] str          The string that is to be returned if non-NULL.
 * @param[in] fallback_str The string that is to be returned if non-NULL and if @a str is NULL.
 *
 * @return A non-NULL string from the list [str, fallback_str, ""], in priority order.
 */
const char * qh_str(const char *__str, const char *__fallback_str) __attribute__((__const__));

/**
 * Safely create a duplicate of a string.
 *
 * Creates a duplicate of the string pointed to by @a str (or pointed to by @a fallback_str if
 * @a str is NULL), and returns a pointer to the new copy.  If both @a str and @a fallback_str are
 * NULL, it returns NULL and sets errno to EINVAL.
 *
 * @param[in] str          The string to be duplicated if non-NULL.
 * @param[in] fallback_str The string to be duplicated if non-NULL and if @a str is NULL.
 *
 * @return A pointer to a copy of a non-NULL string from the list [str, fallback_str], in priority
 *         order, or NULL if both were NULL.  When returning NULL, errno is always set, either to
 *         EINVAL if both strings were NULL, or to a standard errno if an error occurred creating
 *         the duplicate string.
 */
char * qh_strdup(const char *__str, const char *__fallback_str);

/**
 * Safely compare two strings.
 *
 * Compares the string pointed to by str1 to the string pointed to by str2.  A NULL argument is
 * valid, compares smaller to a non-NULL string, and compares equal to another NULL string.
 *
 * @param[in] str1 The first string that you want to compare, or NULL.
 * @param[in] str2 The second string that you want to compare, or NULL.
 *
 * @return -2 if str1 is NULL and str2 is not,
 *         2 if str2 is NULL and str1 is not,
 *         the return value of strcmp if str1 and str2 are not NULL:
 *             - < 0 if str1 is less than str2,
 *             - 0 if str1 is equal to str2,
 *             - > 0 if str1 is greater than str2
 */
int qh_strcmp(const char *__str1, const char *__str2) __attribute__((__pure__));

/**
 * Set the memory to zero.
 *
 * Clears the memory, setting it to zero, while bypassing any possible compiler optimizations that
 * could otherwise prevent the memory from being cleared.
 *
 * @param[out] dst The destination buffer to clear.
 * @param[in] size The size of the destination buffer to clear.
 */
static inline void qh_memclear(void *__dst, size_t __size) QH_ATTR_FUNC_NONNULL_PARAMS(1);
static inline void
qh_memclear(void *__dst, size_t __size)
{
    (void)memset_s(__dst, __size, 0, __size);
}

/**
 * Safely converts a string into an int8_t.
 *
 * This function is similar to strtoll, but does more error checking on the result of the
 * conversion and ensures the result fits in an int8_t.  This simplifies the work required by the
 * caller, which now only needs to validate that the return value from qh_str_to_int8 is EOK.
 *
 * For conversion of strings that are part of a longer one (e.g. a comma separated list), an
 * optional end_offset parameter can be specified, in which case the function will return
 * successfully even if there are trailing characters.  When non-NULL, the end_offset is always
 * updated with the offset of the last character the function parsed.  When NULL, the function will
 * treat trailing characters as invalid, except for characters recognized by isspace.
 *
 * For more information regarding usage, see the documentation for strtoll.
 *
 * @param[in]  str        The string to be converted into a number.
 * @param[in]  base       The base of the number being parsed.
 * @param[out] number     Pointer to the location where the number can be stored.
 * @param[out] end_offset The offset at which parsing was stopped.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and number isn't
 *         updated except for ERANGE).
 * @retval ERANGE The number value was outside of range, but the returned number is either INT8_MAX
 *         or INT8_MIN, whichever is closest to the represented value.
 */
int qh_str_to_int8(const char *__str, int __base, int8_t *__number, size_t *__end_offset);

/**
 * Safely converts a string into an int16_t.
 *
 * This function is similar to strtoll, but does more error checking on the result of the
 * conversion and ensures the result fits in an int16_t.  This simplifies the work required by the
 * caller, which now only needs to validate that the return value from qh_str_to_int16 is EOK.
 *
 * For conversion of strings that are part of a longer one (e.g. a comma separated list), an
 * optional end_offset parameter can be specified, in which case the function will return
 * successfully even if there are trailing characters.  When non-NULL, the end_offset is always
 * updated with the offset of the last character the function parsed.  When NULL, the function will
 * treat trailing characters as invalid, except for characters recognized by isspace.
 *
 * For more information regarding usage, see the documentation for strtoll.
 *
 * @param[in]  str        The string to be converted into a number.
 * @param[in]  base       The base of the number being parsed.
 * @param[out] number     Pointer to the location where the number can be stored.
 * @param[out] end_offset The offset at which parsing was stopped.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and number isn't
 *         updated except for ERANGE).
 * @retval ERANGE The number value was outside of range, but the returned number is either INT16_MAX
 *         or INT16_MIN, whichever is closest to the represented value.
 */
int qh_str_to_int16(const char *__str, int __base, int16_t *__number, size_t *__end_offset);

/**
 * Safely converts a string into an int32_t.
 *
 * This function is similar to strtoll, but does more error checking on the result of the
 * conversion and ensures the result fits in an int32_t.  This simplifies the work required by the
 * caller, which now only needs to validate that the return value from qh_str_to_int32 is EOK.
 *
 * For conversion of strings that are part of a longer one (e.g. a comma separated list), an
 * optional end_offset parameter can be specified, in which case the function will return
 * successfully even if there are trailing characters.  When non-NULL, the end_offset is always
 * updated with the offset of the last character the function parsed.  When NULL, the function will
 * treat trailing characters as invalid, except for characters recognized by isspace.
 *
 * For more information regarding usage, see the documentation for strtoll.
 *
 * @param[in]  str        The string to be converted into a number.
 * @param[in]  base       The base of the number being parsed.
 * @param[out] number     Pointer to the location where the number can be stored.
 * @param[out] end_offset The offset at which parsing was stopped.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and number isn't
 *         updated except for ERANGE).
 * @retval ERANGE The number value was outside of range, but the returned number is either INT32_MAX
 *         or INT32_MIN, whichever is closest to the represented value.
 */
int qh_str_to_int32(const char *__str, int __base, int32_t *__number, size_t *__end_offset);

/**
 * Safely converts a string into an int64_t.
 *
 * This function is similar to strtoll, but does more error checking on the result of the
 * conversion and ensures the result fits in an int64_t.  This simplifies the work required by the
 * caller, which now only needs to validate that the return value from qh_str_to_int64 is EOK.
 *
 * For conversion of strings that are part of a longer one (e.g. a comma separated list), an
 * optional end_offset parameter can be specified, in which case the function will return
 * successfully even if there are trailing characters.  When non-NULL, the end_offset is always
 * updated with the offset of the last character the function parsed.  When NULL, the function will
 * treat trailing characters as invalid, except for characters recognized by isspace.
 *
 * For more information regarding usage, see the documentation for strtoll.
 *
 * @param[in]  str        The string to be converted into a number.
 * @param[in]  base       The base of the number being parsed.
 * @param[out] number     Pointer to the location where the number can be stored.
 * @param[out] end_offset The offset at which parsing was stopped.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and number isn't
 *         updated except for ERANGE).
 * @retval ERANGE The number value was outside of range, but the returned number is either INT64_MAX
 *         or INT64_MIN, whichever is closest to the represented value.
 */
int qh_str_to_int64(const char *__str, int __base, int64_t *__number, size_t *__end_offset);

/**
 * Safely converts a string into a uint8_t
 *
 * This function is similar to strtoull, but does more error checking on the result of the
 * conversion and ensures the result fits in a uint8_t.  This simplifies the work required by the
 * caller, which now only needs to validate that the return value from qh_str_to_uint8 is EOK.
 *
 * For conversion of strings that are part of a longer one (e.g. a comma separated list), an
 * optional end_offset parameter can be specified, in which case the function will return
 * successfully even if there are trailing characters.  When non-NULL, the end_offset is always
 * updated with the offset of the last character the function parsed.  When NULL, the function will
 * treat trailing characters as invalid, except for characters recognized by isspace.
 *
 * For more information regarding usage, see the documentation for strtoull.
 *
 * Contrary to strtoull, this function does not accept a negative sign.
 *
 * @param[in]  str        The string to be converted into a number.
 * @param[in]  base       The base of the number being parsed.
 * @param[out] number     Pointer to the location where the number can be stored.
 * @param[out] end_offset The offset at which parsing was stopped.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and number isn't
 *         updated except for ERANGE).
 * @retval ERANGE The number value was outside of range, but the returned number is UINT8_MAX.
 */
int qh_str_to_uint8(const char *__str, int __base, uint8_t *__number, size_t *__end_offset);

/**
 * Safely converts a string into a uint16_t
 *
 * This function is similar to strtoull, but does more error checking on the result of the
 * conversion and ensures the result fits in a uint16_t.  This simplifies the work required by the
 * caller, which now only needs to validate that the return value from qh_str_to_uint16 is EOK.
 *
 * For conversion of strings that are part of a longer one (e.g. a comma separated list), an
 * optional end_offset parameter can be specified, in which case the function will return
 * successfully even if there are trailing characters.  When non-NULL, the end_offset is always
 * updated with the offset of the last character the function parsed.  When NULL, the function will
 * treat trailing characters as invalid, except for characters recognized by isspace.
 *
 * For more information regarding usage, see the documentation for strtoull.
 *
 * Contrary to strtoull, this function does not accept a negative sign.
 *
 * @param[in]  str        The string to be converted into a number.
 * @param[in]  base       The base of the number being parsed.
 * @param[out] number     Pointer to the location where the number can be stored.
 * @param[out] end_offset The offset at which parsing was stopped.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and number isn't
 *         updated except for ERANGE).
 * @retval ERANGE The number value was outside of range, but the returned number is UINT16_MAX.
 */
int qh_str_to_uint16(const char *__str, int __base, uint16_t *__number, size_t *__end_offset);

/**
 * Safely converts a string into a uint32_t.
 *
 * This function is similar to strtoull, but does more error checking on the result of the
 * conversion and ensures the result fits in a uint32_t.  This simplifies the work required by the
 * caller, which now only needs to validate that the return value from qh_str_to_uint32 is EOK.
 *
 * For conversion of strings that are part of a longer one (e.g. a comma separated list), an
 * optional end_offset parameter can be specified, in which case the function will return
 * successfully even if there are trailing characters.  When non-NULL, the end_offset is always
 * updated with the offset of the last character the function parsed.  When NULL, the function will
 * treat trailing characters as invalid, except for characters recognized by isspace.
 *
 * For more information regarding usage, see the documentation for strtoull.
 *
 * Contrary to strtoull, this function does not accept a negative sign.
 *
 * @param[in]  str        The string to be converted into a number.
 * @param[in]  base       The base of the number being parsed.
 * @param[out] number     Pointer to the location where the number can be stored.
 * @param[out] end_offset The offset at which parsing was stopped.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and number isn't
 *         updated except for ERANGE).
 * @retval ERANGE The number value was outside of range, but the returned number is UINT32_MAX.
 */
int qh_str_to_uint32(const char *__str, int __base, uint32_t *__number, size_t *__end_offset);

/**
 * Safely converts a string into a uint64_t.
 *
 * This function is similar to strtoull, but does more error checking on the result of the
 * conversion and ensures the result fits in a uint64_t.  This simplifies the work required by the
 * caller, which now only needs to validate that the return value from qh_str_to_uint64 is EOK.
 *
 * For conversion of strings that are part of a longer one (e.g. a comma separated list), an
 * optional end_offset parameter can be specified, in which case the function will return
 * successfully even if there are trailing characters.  When non-NULL, the end_offset is always
 * updated with the offset of the last character the function parsed.  When NULL, the function will
 * treat trailing characters as invalid, except for characters recognized by isspace.
 *
 * For more information regarding usage, see the documentation for strtoull.
 *
 * Contrary to strtoull, this function does not accept a negative sign.
 *
 * @param[in]  str        The string to be converted into a number.
 * @param[in]  base       The base of the number being parsed.
 * @param[out] number     Pointer to the location where the number can be stored.
 * @param[out] end_offset The offset at which parsing was stopped.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and number isn't
 *         updated except for ERANGE).
 * @retval ERANGE The number value was outside of range, but the returned number is UINT64_MAX.
 */
int qh_str_to_uint64(const char *__str, int __base, uint64_t *__number, size_t *__end_offset);

/**
 * Safely converts a string representing a size in bytes with an optional size
 * suffix into a uint64_t.
 *
 * The string is interpreted as a decimal (base 10) and if a suffix is specified,
 * the value is multiplied by a corresponding amount.  Valid suffixes are k, m, g, and t,
 * each of which represents the appropriate power of 1024.  Note that suffixes are case
 * insensitive and must come directly after the number, without any intervening whitespace.
 *
 * This function is similar to strtoull, but does more error checking on the result of the
 * conversion and ensures the result fits in a uint64_t.  This simplifies the work required
 * by the caller, which now only needs to validate that the return value from qh_str_to_bytes
 * is EOK.
 *
 * For conversion of strings that are part of a longer one (e.g. a comma separated list), an
 * optional end_offset parameter can be specified, in which case the function will return
 * successfully even if there are trailing characters.  When non-NULL, the end_offset is always
 * updated with the offset of the last character the function parsed.  When NULL, the function will
 * treat trailing characters as invalid, except for characters recognized by isspace.
 *
 * For more information regarding usage, see the documentation for strtoull.
 *
 * @param[in]  str        The string to be converted into a number.
 * @param[out] number     Pointer to the location where the number can be stored.
 * @param[out] end_offset The offset at which parsing was stopped.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and number isn't
 *         updated except for ERANGE).
 * @retval ERANGE The number value was outside of range, but the returned number is UINT64_MAX.
 */
int qh_str_to_bytes(const char *__str, uint64_t *__number, size_t *__end_offset);

/**
 * Safely converts a string into a mode_t.
 *
 * The string is interpreted as an octal (base 8).
 *
 * This function is similar to strtoull, but does more error checking on the result of the
 * conversion and ensures the result fits in a mode_t.  This simplifies the work required by the
 * caller, which now only needs to validate that the return value from qh_str_to_mode is EOK.
 *
 * For conversion of strings that are part of a longer one (e.g. a comma separated list), an
 * optional end_offset parameter can be specified, in which case the function will return
 * successfully even if there are trailing characters.  When non-NULL, the end_offset is always
 * updated with the offset of the last character the function parsed.  When NULL, the function will
 * treat trailing characters as invalid, except for characters recognized by isspace.
 *
 * For more information regarding usage, see the documentation for strtoull.
 *
 * Contrary to strtoull, this function does not accept a negative sign.
 *
 * @param[in]  str        The string to be converted into a mode.
 * @param[out] mode       Pointer to the location where the mode can be stored.
 * @param[out] end_offset The offset at which parsing was stopped.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and mode isn't updated
 *         except for ERANGE).
 * @retval ERANGE The mode value was outside of range, but the returned mode is still set only with
 *         the valid mode bits that were set.
 */
int qh_str_to_mode(const char *__str, mode_t *__mode, size_t *__end_offset);

/**
 * Safely converts a string into an int.
 *
 * The string is interpreted as a decimal (base 10).
 *
 * This function is similar to strtoll, but does more error checking on the result of the
 * conversion and ensures the result fits in an int.  This simplifies the work required by the
 * caller, which now only needs to validate that the return value from qh_str_to_signal is EOK.
 *
 * For conversion of strings that are part of a longer one (e.g. a comma separated list), an
 * optional end_offset parameter can be specified, in which case the function will return
 * successfully even if there are trailing characters.  When non-NULL, the end_offset is always
 * updated with the offset of the last character the function parsed.  When NULL, the function will
 * treat trailing characters as invalid, except for characters recognized by isspace.
 *
 * For more information regarding usage, see the documentation for strtoll.
 *
 * @param[in]  str        The string to be converted into a signal number.
 * @param[out] number     Pointer to the location where the signal can be stored.
 * @param[out] end_offset The offset at which parsing was stopped.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and num isn't updated).
 */
int qh_str_to_signal(const char *__str, int *__number, size_t *__end_offset);

/**
 * Safely converts a string into a uid_t.
 *
 * This function first checks the password database to determine if the string corresponds
 * to a username.  If it does, the uid for that account is returned.  Otherwise, the function
 * attempts to interpret the string as a decimal number (base 10) similar to strtoll, but
 * does more error checking on the result of the conversion and ensures the result fits in
 * a uid_t.  This simplifies the work required by the caller, which now only needs to validate
 * that the return value from qh_str_username_to_uid is EOK.
 *
 * For more information regarding usage, see the documentation for strtoll.
 *
 * @param[in]  str The string to be converted into a uid.
 * @param[out] uid Pointer to the location where the uid can be stored.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and uid isn't updated).
 */
#if !defined(__MINGW32__) && !defined(__MINGW64__)
int qh_str_username_to_uid(const char *__str, uid_t *__uid);
#endif

/**
 * Safely converts a string into a gid_t.
 *
 * This function first checks the password database to determine if the string corresponds
 * to a groupname.  If it does, the gid for that group is returned.  Otherwise, the function
 * attempts to interpret the string as a decimal number (base 10) similar to strtoll, but
 * does more error checking on the result of the conversion and ensures the result fits in
 * a gid_t.  This simplifies the work required by the caller, which now only needs to validate
 * that the return value from qh_str_groupname_to_gid is EOK.
 *
 * For more information regarding usage, see the documentation for strtoll.
 *
 * @param[in]  str The string to be converted into a gid.
 * @param[out] gid Pointer to the location where the gid can be stored.
 *
 * @return EOK on success or a standard errno on failure (errno is also set, and gid isn't updated).
 */
#if !defined(__MINGW32__) && !defined(__MINGW64__)
int qh_str_groupname_to_gid(const char *__str, gid_t *__gid);
#endif

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QH_STRING_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qh/public/qh/string.h $ $Rev: 897545 $")
#endif
