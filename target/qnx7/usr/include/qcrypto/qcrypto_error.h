/*
 * $QNXLicenseC:
 * Copyright (c) 2016-2019 QNX Software Systems. All Rights Reserved.
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
 * @file qcrypto_error.h
 * @brief QNX cryptography library error handling API
 *
 * This library provides error handling functions and the error codes that the cryptography library
 * uses and returns.
 */

#ifndef _QCRYPTO_QCRYPTO_ERROR_H_INCLUDED
#define _QCRYPTO_QCRYPTO_ERROR_H_INCLUDED

/**
 * @defgroup QCRYPTO_ERROR_CODES QNX cryptography library error codes
 *
 * The error codes returned by the library and its plugins.
 *
 * @{
 */
#define QCRYPTO_R_EOK						0		/*!< Success. */

/* General Error Codes */

#define QCRYPTO_R_EINVAL					-1		/*!< Invalid input parameters. */
#define QCRYPTO_R_ENOMEM					-2		/*!< Failed to allocate memory. */
#define QCRYPTO_R_ENOENT					-3		/*!< No entry found. */
#define QCRYPTO_R_EEXIST					-4		/*!< Entry already exists. */
#define QCRYPTO_R_EIO						-5		/*!< I/O error. */
#define QCRYPTO_R_ELIBBAD					-6		/*!< Library error. */
#define QCRYPTO_R_EFILE						-7		/*!< File access or IO error (check @c errno). */
#define QCRYPTO_R_ENOTSUP					-8		/*!< Not supported. */

/* Library Error Codes */

#define QCRYPTO_R_NOT_INIT					-100	/*!< Library was not initalized. */
#define QCRYPTO_R_INIT_FAIL					-101	/*!< Library initialization failed. */
#define QCRYPTO_R_PLUGIN_COMPAT				-102	/*!< Plugin version is not compatible with library. */
#define QCRYPTO_R_CFG_LOAD_FAIL				-103	/*!< Configuration file loading failed. */
#define QCRYPTO_R_CFG_INVALID				-104	/*!< Configuration is invalid. */
#define QCRYPTO_R_DLL_LOAD_FAIL				-105	/*!< Plugin DLL failed to load. */
#define QCRYPTO_R_INVALID_PLUGIN			-106	/*!< Invalid plugin. */
#define QCRYPTO_R_PLUGIN_LOADED				-107	/*!< Plugin already loaded. */
#define QCRYPTO_R_PLUGIN_ERROR				-108	/*!< Plugin initialization or internal error. */
#define QCRYPTO_R_LIB_LOAD_FAIL				-109	/*!< Dynamic library failed to load. */

/* Key Error Codes */

#define QCRYPTO_R_KEY_INVALID				-300	/*!< Invalid key. */
#define QCRYPTO_R_KEY_OPTS_INVALID			-301	/*!< Invalid key parameters. */
#define QCRYPTO_R_KEY_SIZE_INVALID			-302	/*!< Invalid key size. */
#define QCRYPTO_R_KEY_FMT_INVALID			-303	/*!< Invalid key format. */
#define QCRYPTO_R_KEY_TYPE_INVALID			-304	/*!< Invalid key type. */
#define QCRYPTO_R_KEY_LEN_INVALID			-305	/*!< Invalid key length. */

/* Algorithm General Codes */

#define QCRYPTO_R_ALG_ENOENT				-400	/*!< Algorithm not found in the available plugins. */
#define QCRYPTO_R_ALG_ETYPE					-401	/*!< Invalid algorithm type. */
#define QCRYPTO_R_ALG_ENOTSUP				-402	/*!< Algorithm doesn't support the function. */
#define QCRYPTO_R_ALG_ERROR					-403	/*!< Algorithm encountered an internal error. */
#define QCRYPTO_R_ALG_IV_INVALID			-404	/*!< Algorithm initialization vector (IV) is invalid. */
#define QCRYPTO_R_ALG_IV_LEN_INVALID		-405	/*!< Algorithm IV length is invalid. */
#define QCRYPTO_R_ALG_TAG_INVALID			-406	/*!< Algorithm tag is invalid. */
#define QCRYPTO_R_ALG_TAG_LEN_INVALID		-407	/*!< Algorithm tag length is invalid. */
#define QCRYPTO_R_ALG_DIGEST_LEN_INVALID	-408	/*!< Algorithm digest length is invalid. */
#define QCRYPTO_R_ALG_SIG_LEN_INVALID		-409	/*!< Algorithm signature length is invalid. */
#define QCRYPTO_R_ALG_PAD_INVALID			-410	/*!< Invalid padding mode. */
#define QCRYPTO_R_ALG_SALT_LEN_INVALID		-411	/*!< Invalid salt length. */
#define QCRYPTO_R_ALG_INVALID_STATE			-412	/*!< Invalid operation in current algorithm state. */

/* Cipher Error Codes */
#define QCRYPTO_R_XTS_UNIT_INVALID			-500	/*!< XTS unit size is invalid. */
#define QCRYPTO_R_XTS_SIZE_INVALID			-501	/*!< XTS encrypt or decrypt size is invalid. */
#define QCRYPTO_R_INPUT_BLOCK_SIZE			-502	/*!< Cipher input is not a multiple of the block size. */
#define QCRYPTO_R_INVALID_CIPHER_ACTION		-503	/*!< Invalid cipher action. */
#define QCRYPTO_R_BAD_DECRYPTION			-504	/*!< Bad cipher decryption. */

/* Padding Error Codes */
#define QCRYPTO_R_PAD_BUF_LEN_INVALID		-600	/*!< Padding buffer length is too short. */
#define QCRYPTO_R_PAD_INVALID				-601	/*!< Invalid padding detected. */
#define QCRYPTO_R_INVALID_PADDING			-602	/*!< Invalid padding mode. */

#ifndef OMIT_FROM_DOXYGEN
/* X509 Error Code */
#define QCRYPTO_R_X509_INVALID				-700	/*!< Invalid X.509 library. */
#define QCRYPTO_R_X509_FMT_INVALID			-701	/*!< Invalid X.509 format. */
#define QCRYPTO_R_X509_SERIAL_INVALID		-702	/*!< X.509 serial number is invalid. */
#define QCRYPTO_R_X509_ISSUER_INVALID		-703	/*!< X.509 issuer is invalid or empty. */
#define QCRYPTO_R_X509_SUBJECT_INVALID		-704	/*!< X.509 subject is invalid or empty. */
#define QCRYPTO_R_X509_PUBKEY_INVALID		-705	/*!< X.509 public key is invalid or empty. */
#define QCRYPTO_R_X509_PRIVKEY_INVALID		-706	/*!< X.509 private key is invalid or empty. */
#define QCRYPTO_R_X509_VALIDITY_INVALID		-707	/*!< X.509 validity period is invalid or empty. */
#define QCRYPTO_R_X509_HASH_INVALID			-708	/*!< X.509 hash signing algorithm is invalid. */
#define QCRYPTO_R_X509_EXT_OID_INVALID		-709	/*!< X.509 extension OID is invalid. */
#define QCRYPTO_R_X509_EXT_DATA_INVALID		-710	/*!< X.509 extension data is invalid. */
#endif
/** @} */

/**
 * @brief Return the string associated with an error code
 *
 * @param[in] qcrypto_err The @c qcrypto library error code.
 *
 * @return The string that matches the error code or @c unknown for an unrecognized error code.
 */
const char *qcrypto_strerror(int qcrypto_err);


#endif /* _QCRYPTO_QCRYPTO_ERROR_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qcrypto/qcrypto/public/qcrypto/qcrypto_error.h $ $Rev: 909140 $")
#endif
