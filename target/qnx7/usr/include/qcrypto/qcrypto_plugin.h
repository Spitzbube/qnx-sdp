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
 * @file qcrypto_plugin.h
 * @brief QNX cryptography library plugin API
 *
 * This API allows you to create custom plugins that allow the cryptography library to provide
 * additional cryptographic primitives.
 */

#ifndef _QCRYPTO_QCRYPTO_PLUGIN_H_INCLUDED
#define _QCRYPTO_QCRYPTO_PLUGIN_H_INCLUDED

#include <inttypes.h>
#include <stdarg.h>
#include <unistd.h>

#include <lib/compat.h>

#include <sys/cdefs.h>

#include <qcrypto/qcrypto.h>
#include <qcrypto/qcrypto_keys.h>
#include <qcrypto/qcrypto_x509.h>
#include <qcrypto/qcrypto_x509_exts.h>

/** @cond */
__BEGIN_DECLS
/** @endcond */

/**** Defines ****/

/* forward decls */
typedef struct _qcrypto_algorithm qcrypto_algorithm_t;

/**
 * @brief Algorithm state context
 */
typedef struct _qcrypto_state_ctx {
	void						*data;	/*!< An algorithm-specific data holder. */
	const qcrypto_algorithm_t	*alg;	/*!< The algorithm attached to this context for reference. */
} qcrypto_state_ctx_t;

/**** Plugin Handling ****/

/**
 * @brief Initialize a plugin
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_plugin_op_init)(void);

/**
 * @brief Un-initialize a plugin
 */
typedef void (*qcrypto_plugin_op_uninit)(void);

/**
 * @brief Get a plugin description
 *
 * @return The plugin description as a string.
 */
typedef const char* (*qcrypto_plugin_op_get_description)(void);

/**
 * @brief Global plugin functions
 */
typedef struct _qcrypto_plugin_ops {
	uint16_t							version;	/*!< The plugin version. Always set to @c QCRYPTO_PLUGIN_VERSION. */
	qcrypto_plugin_op_init				init;		/*!< The plugin initialization function. */
	qcrypto_plugin_op_uninit			uninit;		/*!< The plugin un-initialization function. */
	qcrypto_plugin_op_get_description	get_desc;	/*!< The function that gets a plugin. */
} qcrypto_plugin_ops_t;

/**** Key Handling ****/

/**
 * @brief Key types
 */
typedef enum {
	QCRYPTO_INVALID_KEY = 0,	/*!< Invalid key. */
	QCRYPTO_SYMMETRIC_KEY,		/*!< Symmetric key. */
	QCRYPTO_PUBLIC_KEY,			/*!< Public key. */
	QCRYPTO_PRIVATE_KEY,		/*!< Private key. */
	QCRYPTO_MAX_KEY,			/*!< Maximum key type. */
	QCRYPTO_CUSTOM_KEY = 100,	/*!< Custom key types. */
} qcrypto_key_type_t;

/**
 * @brief Key algorithm types
 */
typedef enum {
	QCRYPTO_INVALID_KEY_ALG = 0,	/*!< Invalid key algorithm. */
	QCRYPTO_SYMMETRIC_KEY_ALG,		/*!< Symmetric key algorithm. */
	QCRYPTO_RSA_KEY_ALG,			/*!< RSA key algorithm. */
	QCRYPTO_DSA_KEY_ALG,			/*!< DSA key algorithm. */
	QCRYPTO_DH_KEY_ALG,				/*!< Diffie-Hellman key algorithm. */
	QCRYPTO_EC_KEY_ALG,				/*!< Elliptic curve key algorithm. */
	QCRYPTO_ED25519_KEY_ALG,		/*!< Ed25519 curve key algorithm. */
	QCRYPTO_ED448_KEY_ALG,			/*!< Ed448 curve key algorithm. */
	QCRYPTO_MAX_KEY_ALG,			/*!< Maximum key algorithm. */
	QCRYPTO_CUSTOM_KEY_ALG = 100,	/*!< Custom key algorithms. */
} qcrypto_key_alg_type_t;

/**
 * @brief Key context
 */
typedef struct _qcrypto_key_ctx {
	qcrypto_key_type_t		type;		/*!< The key type. */
	qcrypto_key_alg_type_t	alg_type;	/*!< The key algorithm type. */
	uint8_t					*key;		/*!< The key data. */
	size_t					keysize;	/*!< The key size. */
} qcrypto_key_ctx_t;

/**
 * @brief Generate a symmetric key
 *
 * @details Plugins implement this function to generate symmetric keys. It should be set for
 *          symmetric key algorithms only.
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] kctx The key context object.
 * @param[in] keysize The key size in bytes.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_keygen_op_key_gen)(qcrypto_state_ctx_t *sctx, qcrypto_key_ctx_t *kctx,
		size_t keysize);

/**
 * @brief Initialize a key pair
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] kargs The key arguments.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_keygen_op_keypair_init)(qcrypto_state_ctx_t *sctx, const qcrypto_key_args_t *kargs);

/**
 * @brief Initialize a key pair using parameters
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] params The key parameters buffer.
 * @param[in] size The key parameters buffer size.
 * @param[in] fmt The key parameters encoding format.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_keygen_op_keypair_init_params)(qcrypto_state_ctx_t *sctx, const uint8_t *params,
		size_t size, qcrypto_key_format_t fmt);

/**
 * @brief Generate a key pair
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] pubkctx The public key context object.
 * @param[in] privkctx The private key context object.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_keygen_op_keypair_gen)(qcrypto_state_ctx_t *sctx, qcrypto_key_ctx_t *pubkctx,
		qcrypto_key_ctx_t *privkctx);

/**
 * @brief Retrieve a public key from a private key.
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] privkctx The private key context object.
 * @param[in] pubkctx The public key context object (optional).
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_keygen_op_get_pubkey)(qcrypto_state_ctx_t *sctx, qcrypto_key_ctx_t *privkctx,
		qcrypto_key_ctx_t *pubkctx);

/**
 * @brief Import a key
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] kctx The key context object.
 * @param[in] kbuf The key buffer.
 * @param[in] ksize The key buffer size.
 * @param[in] type The key type.
 * @param[in] fmt The key encoding format.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_keygen_op_import_raw)(qcrypto_state_ctx_t *sctx, qcrypto_key_ctx_t *kctx,
		const uint8_t *kbuf, size_t ksize, qcrypto_key_type_t type, qcrypto_key_format_t fmt);

/**
 * @brief Export a key.
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] kctx The key context object.
 * @param[out] kbuf The key buffer.
 * @param[in,out] ksize The combined size of the key buffer and retrieved key.
 * @param[in] fmt The key encoding format.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_keygen_op_export_raw)(qcrypto_state_ctx_t *sctx, qcrypto_key_ctx_t *kctx,
		uint8_t *kbuf, size_t *ksize, qcrypto_key_format_t fmt);

/**
 * @brief Release a key and its resources
 *
 * @param[in] kctx The cryptographic key context object.
 */
typedef void (*qcrypto_keygen_op_release)(qcrypto_key_ctx_t *kctx);

/**
 * @brief Key generation functions
 */
typedef struct _qcrypto_keygen_ops {
	qcrypto_keygen_op_key_gen				key_gen;				/*!< The symmetric key generation function. */
	qcrypto_keygen_op_keypair_init			keypair_init;			/*!< The key pair initialization function.*/
	qcrypto_keygen_op_keypair_init_params	keypair_init_params;	/*!< The key pair initialization with preset parameters function. */
	qcrypto_keygen_op_keypair_gen			keypair_gen;			/*!< The public-private key pair generation function. */
	qcrypto_keygen_op_get_pubkey			get_pubkey;				/*!< The function that extracts a public key from a private one. */
	qcrypto_keygen_op_import_raw			import_raw;				/*!< The function that imports a key from a raw key blob. */
	qcrypto_keygen_op_export_raw			export_raw;				/*!< The raw key blob retrieval function. */
	qcrypto_keygen_op_release				release;				/*!< The key release function. */
} qcrypto_keygen_ops_t;

/**
 * @brief Key generation parameters
 */
typedef struct _qcrypto_keygen_params {
	qcrypto_key_alg_type_t		alg_type;	/*!< The key algorithm type. */
} qcrypto_keygen_params_t;

/**
 * @brief Free the specified key context
 *
 * This helper function frees the key data and sets it to zero.
 *
 * @param[in] kctx The key context object.
 */
void qcrypto_key_ctx_free(qcrypto_key_ctx_t *kctx);

/**
 * @brief Key generation object
 */
typedef struct _qcrypto_keygen {
	qcrypto_keygen_params_t		params;	/*!< The key parameters. */
	qcrypto_keygen_ops_t		ops;	/*!< The key functions. */
} qcrypto_keygen_t;

/**** Digest Handling ****/

/**
 * @brief Initialize a digest object
 *
 * This function initializes or resets the digest object's state.
 *
 * @param[in] sctx The algorithm state context object.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_digest_op_init)(qcrypto_state_ctx_t *sctx);

/**
 * @brief Update a digest object
 *
 * This function updates a digest by supplying data.
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] data The data buffer.
 * @param[in] size The data buffer size.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_digest_op_update)(qcrypto_state_ctx_t *sctx, const uint8_t *data, size_t size);

/**
 * @brief Finalize a digest object
 *
 * This function finalizes the state of a digest.
 *
 * @param[in] sctx The algorithm state context object.
 * @param[out] digest The digest buffer.
 * @param[in,out] size The combined size of the digest buffer and retrieved digest.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_digest_op_final)(qcrypto_state_ctx_t *sctx, uint8_t *digest, size_t *size);

/**
 * @brief Copy a digest state object
 *
 * This function copies the current, non-finalized state of a digest so that the digest operation
 * can be continued from the last update.
 *
 * @param[in] sctx The source algorithm state context object.
 * @param[in] new_sctx The destination algorithm state context object.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_digest_op_copy)(qcrypto_state_ctx_t *sctx, qcrypto_state_ctx_t *new_sctx);

/**
 * @brief Digest functions
 */
typedef struct _qcrypto_digest_ops {
	qcrypto_digest_op_init		init;		/*!< The digest initialization function. */
	qcrypto_digest_op_update	update;		/*!< The digest update function. */
	qcrypto_digest_op_final		final;		/*!< The digest finalization function. */
	qcrypto_digest_op_copy		copy;		/*!< The digest state copy function. */
} qcrypto_digest_ops_t;

/**
 * @brief Digest parameters
 */
typedef struct _qcrypto_digest_params {
	size_t		digestsize;		/*!< The digest size in bytes. */
	size_t		blocksize;		/*!< The blocksize in bytes. */
} qcrypto_digest_params_t;

/**
 * @brief Digest object
 */
typedef struct _qcrypto_digest {
	qcrypto_digest_params_t		params;	/*!< The digest parameters. */
	qcrypto_digest_ops_t		ops;	/*!< The digest functions. */
} qcrypto_digest_t;

/**** RNG Handling ****/

/**
 * @brief Initialize an RNG algorithm
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] rargs The RNG arguments.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_rng_op_init)(qcrypto_state_ctx_t *sctx, const qcrypto_rng_args_t *rargs);

/**
 * @brief Seed the random number generator (RNG) algorithm
 *
 * This function seeds the RNG algorithm with an initial value.
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] seed The seed buffer.
 * @param[in] size The seed buffer size.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_rng_op_seed)(qcrypto_state_ctx_t *sctx, const uint8_t *seed, size_t size);

/**
 * @brief Extract a value from the random number generator (RNG) algorithm
 *
 * This function is used to extract a random-sized value from the RNG algorithm.
 *
 * @param[in] sctx The algorithm state context object.
 * @param[out] buf The buffer to store the random bytes in.
 * @param[in] size The buffer size.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_rng_op_bytes)(qcrypto_state_ctx_t *sctx, uint8_t *buf, size_t size);

/**
 * @brief RNG functions
 */
typedef struct _qcrypto_rng_ops {
	qcrypto_rng_op_init			init;		/*!< The RNG initialization function. */
	qcrypto_rng_op_seed			seed;		/*!< The RNG seed function. */
	qcrypto_rng_op_bytes		bytes;		/*!< The RNG bytes extraction function. */
} qcrypto_rng_ops_t;

/**
 * @brief RNG parameters
 */
typedef struct _qcrypto_rng_params {
	size_t		security;		/*!< The security strength. */
} qcrypto_rng_params_t;

/**
 * @brief RNG object
 */
typedef struct _qcrypto_rng {
	qcrypto_rng_params_t		params;	/*!< The RNG parameters. */
	qcrypto_rng_ops_t			ops;	/*!< The RNG functions. */
} qcrypto_rng_t;


/**** Cipher Handling ****/

/**
 * @brief Initialize a cipher algorithm
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] kctx The key context object.
 * @param[in] cargs The cipher arguments.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_cipher_op_init)(qcrypto_state_ctx_t *sctx, const qcrypto_key_ctx_t *kctx,
		const qcrypto_cipher_args_t *cargs);

/**
 * @brief Encrypt or decrypt data

 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] in The input data.
 * @param[in] insize The size of the input data.
 * @param[out] out The output data.
 * @param[in, out] outsize The size of the output data and encrypted or decrypted data combined.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_cipher_op_encrypt_decrypt)(qcrypto_state_ctx_t *sctx, const uint8_t *in, size_t insize,
		uint8_t *out, size_t *outsize);

/**
 * @brief Add additional authenticated data (AAD) to a cipher
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] aad The AAD data.
 * @param[in] aadsize The size of the AAD data.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_cipher_op_aad)(qcrypto_state_ctx_t *sctx, const uint8_t *aad, size_t aadsize);

/**
 * @brief Retrieve a cipher tag
 *
 * @param[in] sctx The algorithm state context object.
 * @param[out] tag The tag data.
 * @param[in,out] tagsize The combined size of the tag data and retrieved tag.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_cipher_op_get_tag)(qcrypto_state_ctx_t *sctx, uint8_t *tag, size_t *tagsize);

/**
 * @brief Finalize a cipher
 *
 * @param[in] sctx The algorithm state context object.
 * @param[out] out The output data.
 * @param[in,out] outsize The combined size of the output data and the encrypted or decrypted data.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_cipher_op_final)(qcrypto_state_ctx_t *sctx, uint8_t *out, size_t *outsize);

/**
 * @brief Cipher functions
 */
typedef struct _qcrypto_cipher_ops {
	qcrypto_cipher_op_init				init;		/*!< Initialization. */
	qcrypto_cipher_op_encrypt_decrypt	encrypt;	/*!< Encryption. */
	qcrypto_cipher_op_encrypt_decrypt	decrypt;	/*!< Decryption. */
	qcrypto_cipher_op_aad				aad;		/*!< Add AAD. */
	qcrypto_cipher_op_get_tag			get_tag;	/*!< Get a tag. */
	qcrypto_cipher_op_final				final;		/*!< Finalize. */
} qcrypto_cipher_ops_t;

/**
 * @brief Cipher parameters
 */
typedef struct _qcrypto_cipher_params {
	size_t	keysize;	/*!< The key size in bytes. */
	size_t	ivsize;		/*!< The default initialization vector (IV) size in bytes. */
	size_t	tagsize;	/*!< The default tag size in bytes. */
	size_t	blocksize;	/*!< The cipher blocksize in bytes. */
} qcrypto_cipher_params_t;

/**
 * @brief Cipher object
 */
typedef struct _qcrypto_cipher {
	qcrypto_cipher_params_t		params;	/*!< The cipher parameters. */
	qcrypto_cipher_ops_t		ops;	/*!< The cipher functions. */
} qcrypto_cipher_t;

/**** MAC Handling ****/

/**
 * @brief Initialize a MAC algorithm
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] kctx The key context object.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_mac_op_init)(qcrypto_state_ctx_t *sctx, const qcrypto_key_ctx_t *kctx);

/**
 * @brief Update a MAC object
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] data The data buffer.
 * @param[in] size The data buffer size.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_mac_op_update)(qcrypto_state_ctx_t *sctx, const uint8_t *data, size_t size);

/**
 * @brief Finalize a MAC object
 *
 * @param[in] sctx The algorithm state context object.
 * @param[out] tag The tag buffer.
 * @param[in,out] size The combined size of the tag buffer and retrieved tag.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_mac_op_final)(qcrypto_state_ctx_t *sctx, uint8_t *tag, size_t *size);

/**
 * @brief MAC functions
 */
typedef struct _qcrypto_mac_ops {
	qcrypto_mac_op_init			init;		/*!< The MAC initialization function. */
	qcrypto_mac_op_update		update;		/*!< The MAC encryption function. */
	qcrypto_mac_op_final		final;		/*!< The MAC finalization function. */
} qcrypto_mac_ops_t;

/**
 * @brief MAC parameters
 */
typedef struct _qcrypto_mac_params {
	size_t	tagsize;	/*!< The MAC tag size in bytes. */
	size_t	keysize;	/*!< The key size in bytes. */
} qcrypto_mac_params_t;

/**
 * @brief MAC object
 */
typedef struct _qcrypto_mac {
	qcrypto_mac_params_t	params;	/*!< The MAC parameters. */
	qcrypto_mac_ops_t		ops;	/*!< The MAC functions. */
} qcrypto_mac_t;

/**** KDF Handling ****/

/**
 * @brief Generate a symmetric key using a key derivation function (KDF)
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] kctx The key context object.
 * @param[in] keysize The size of key to generate, in bytes.
 * @param[in] kdfargs The KDF arguments.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_kdf_op_gen)(qcrypto_state_ctx_t *sctx, qcrypto_key_ctx_t *kctx, size_t keysize,
		const qcrypto_kdf_args_t *kdfargs);

/**
 * @brief KDF functions
 */
typedef struct _qcrypto_kdf_ops {
	qcrypto_kdf_op_gen			gen;	/*!< The KDF generation function. */
} qcrypto_kdf_ops_t;

/**
 * @brief KDF object
 */
typedef struct _qcrypto_kdf {
	qcrypto_kdf_ops_t		ops;	/*!< KDF functions. */
} qcrypto_kdf_t;

/**** Signature Handling ****/

/**
 * @brief Initialize a signature
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] kctx The key context object to use to sign (private key) or verify (public key).
 * @param[in] sargs The signature arguments.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_signature_op_init)(qcrypto_state_ctx_t *sctx, const qcrypto_key_ctx_t *kctx,
		const qcrypto_signature_args_t *sargs);

/**
 * @brief Update a signature digest
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] data The data buffer.
 * @param[in] size The data buffer size.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_signature_op_update)(qcrypto_state_ctx_t *sctx, const uint8_t *data, size_t size);

/**
 * @brief Sign a signature
 *
 * @param[in] sctx The algorithm state context object.
 * @param[out] sig The signature buffer.
 * @param[in,out] sigsize The combined size of the signature buffer and retrieved signature.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_signature_op_sign)(qcrypto_state_ctx_t *sctx, uint8_t *sig, size_t *sigsize);

/**
 * @brief Sign a signature using a single operation
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] data The input data.
 * @param[in] size The input data size.
 * @param[out] sig The signature buffer.
 * @param[in,out] sigsize The combined size of the signature buffer and retrieved signature.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_signature_op_sign_oneshot)(qcrypto_state_ctx_t *sctx, const uint8_t *data,
		size_t size, uint8_t *sig, size_t *sigsize);

/**
 * @brief Sign a signature digest
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] digest The input digest.
 * @param[in] size The input digest size.
 * @param[out] sig The signature buffer.
 * @param[in,out] sigsize The combined size of the signature buffer and retrieved signature.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_signature_op_sign_nodgst)(qcrypto_state_ctx_t *sctx, const uint8_t *digest,
		size_t size, uint8_t *sig, size_t *sigsize);

/**
 * @brief Verify a signature
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] sig The signature buffer.
 * @param[in] sigsize The signature buffer size.
 * @param[out] status The verification result.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_signature_op_verify)(qcrypto_state_ctx_t *sctx, const uint8_t *sig,
		size_t sigsize, int *status);

/**
 * @brief Verify a signature in a single operation
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] data The input data.
 * @param[in] size The input data size.
 * @param[in] sig The signature buffer.
 * @param[in] sigsize The signature buffer size.
 * @param[out] status The verification result.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_signature_op_verify_oneshot)(qcrypto_state_ctx_t *sctx, const uint8_t *data,
		size_t size, const uint8_t *sig, size_t sigsize, int *status);

/**
 * @brief Verify a signature digest
 *
 * @param[in] sctx The algorithm state context object.
 * @param[in] digest The input digest.
 * @param[in] size The input digest size.
 * @param[in] sig The signature buffer.
 * @param[in] sigsize The signature buffer size.
 * @param[out] status The verification result.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_signature_op_verify_nodgst)(qcrypto_state_ctx_t *sctx, const uint8_t *data,
		size_t size, const uint8_t *sig, size_t sigsize, int *status);

/**
 * @brief Get the signature size
 *
 * @param[in] sctx The algorithm state context object.
 *
 * @return The signature size in bytes.
 */
typedef size_t (*qcrypto_signature_op_sigsize)(qcrypto_state_ctx_t *sctx);

/**
 * @brief Signature functions
 */
typedef struct _qcrypto_signature_ops {
	qcrypto_signature_op_init			init;			/*!< The initialization function. */
	qcrypto_signature_op_update			update;			/*!< The update function. */
	qcrypto_signature_op_sign			sign;			/*!< The signing function. */
	qcrypto_signature_op_sign_oneshot	sign_oneshot;	/*!< The function that signs a signature in single operation. */
	qcrypto_signature_op_sign_nodgst	sign_nodgst;	/*!< The digest signing function. */
	qcrypto_signature_op_verify			verify;			/*!< The signature verification function. */
	qcrypto_signature_op_verify_oneshot	verify_oneshot;	/*!< The function that verifies a signature in a single operation. */
	qcrypto_signature_op_verify_nodgst	verify_nodgst;	/*!< The digest verification function. */
	qcrypto_signature_op_sigsize		size;			/*!< The function that gets the signature size. */
} qcrypto_signature_ops_t;

/**
 * @brief Signature parameters
 *
 * @details If the @c sigsize value is set to @c 0,
 * it is dynamically queried through the
 * @ref qcrypto_signature_ops_t.size function.
 * This configuration is for algorithms where the signature
 * size is not constant (for example, the RSA signature size
 * depends on the key size).
 */
typedef struct _qcrypto_signature_params {
	size_t	sigsize;	/*!< The signature size. */
} qcrypto_signature_params_t;

/**
 * @brief Signature object
 */
typedef struct _qcrypto_signature {
	qcrypto_signature_params_t	params;	/*!< The signature parameters. */
	qcrypto_signature_ops_t		ops;	/*!< The signature functions. */
} qcrypto_signature_t;

/**** X509 Certificate Handling ****/

/**
 * @brief X.509 object context
 */
typedef struct _qcrypto_x509_ctx {
	uint8_t					*x509;		/*!< The X.509 data. */
	size_t					size;		/*!< The X.509 size. */
} qcrypto_x509_ctx_t;

/** @cond */
/*
 * X509 operations
 *
 * NOTE: These functions are experimental and will not appear in release versions for now
 *
 * That's why no docs are written for them.
 */
typedef int (*qcrypto_x509_op_pubkey)(qcrypto_state_ctx_t *sctx, qcrypto_x509_ctx_t *xctx,
		qcrypto_key_ctx_t *kctx);
typedef int (*qcrypto_x509_op_import_raw)(qcrypto_state_ctx_t *sctx, qcrypto_x509_ctx_t *xctx,
		const uint8_t *xbuf, size_t xsize, qcrypto_x509_format_t fmt);
typedef int (*qcrypto_x509_op_export_raw)(qcrypto_state_ctx_t *sctx, qcrypto_x509_ctx_t *xctx,
		uint8_t *xbuf, size_t *xsize, qcrypto_x509_format_t fmt);
typedef int (*qcrypto_x509_op_release)(qcrypto_x509_ctx_t *xctx);
typedef int (*qcrypto_x509_op_create_selfsigned)(qcrypto_state_ctx_t *sctx, qcrypto_x509_ctx_t *xctx,
		const qcrypto_key_ctx_t *pubkctx, const qcrypto_key_ctx_t *privkctx,
		const qcrypto_x509_info_t * const x509_info);
typedef int (*qcrypto_x509_op_create)(qcrypto_state_ctx_t *sctx, qcrypto_x509_ctx_t *xctx,
		const qcrypto_key_ctx_t *pubkctx, const qcrypto_x509_ctx_t *caxctx,
		const qcrypto_key_ctx_t *caprivkctx, const qcrypto_x509_info_t * const x509_info);
typedef int (*qcrypto_x509_op_selfsigned)(qcrypto_state_ctx_t *sctx, qcrypto_x509_ctx_t *xctx,
		int * const selfsigned);

/** @endcond */

/**
 * @brief X509 operations
 */
typedef struct _qcrypto_x509_ops {
	qcrypto_x509_op_pubkey				pubkey;				/*!< extract pubkey from x509 */
	qcrypto_x509_op_import_raw			import_raw;			/*!< make x509 from raw x509 blob */
	qcrypto_x509_op_export_raw			export_raw;			/*!< retrieve raw x509 blob */
	qcrypto_x509_op_release				release;			/*!< x509 release function */
	qcrypto_x509_op_create_selfsigned	create_selfsigned;	/*!< x509 self-signed creation function */
	qcrypto_x509_op_create				create;				/*!< x509 creation function */
	qcrypto_x509_op_selfsigned			selfsigned;			/*!< x509 self signed verification function */
} qcrypto_x509_ops_t;

/**
 * @brief Free a X509 context
 *
 * @param[in] xctx X509 context object
 */
void qcrypto_x509_ctx_free(qcrypto_x509_ctx_t *xctx);

/**
 * @brief X509 object
 */
typedef struct _qcrypto_x509_cert {
	qcrypto_x509_ops_t		ops;	/*!< x509 functions */
} qcrypto_x509_cert_t;


/**** Algorithm Handling ****/

/**
 * @brief Algorithm types
 */
typedef enum {
	QCRYPTO_DIGEST_TYPE = 1,
	QCRYPTO_RNG_TYPE,
	QCRYPTO_KEYGEN_TYPE,
	QCRYPTO_CIPHER_TYPE,
	QCRYPTO_MAC_TYPE,
	QCRYPTO_KDF_TYPE,
	QCRYPTO_SIGNATURE_TYPE,
#ifndef OMIT_FROM_DOXYGEN
	QCRYPTO_X509_TYPE,
#endif
} qcrypto_algorithm_type_t;

/**
 * @brief Initialize an algorithm
 *
 * This function is called when an algorithm is requested
 * to allow plugins to allocate resources and prepare the
 * algorithm for use. The algorithm parameters are passed
 * back to the function so that plugins can use them if they
 * need to.
 *
 * @param[in] sctx The algorithm state context object.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_algorithm_op_init)(qcrypto_state_ctx_t *sctx);

/**
 * @brief Un-initialize an algorithm
 *
 * This function signals that the algorithm object has
 * been released by the user and the plugin can clean
 * up the resources associated with it.
 *
 * @param[in] sctx The algorithm state context object.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
typedef int (*qcrypto_algorithm_op_uninit)(qcrypto_state_ctx_t *sctx);

/**
 * @brief Maximum algorithm name including NUL terminator
 */
#define QCRYPTO_ALG_NAME_MAX	32

/**
 * @brief Algorithm object
 */
typedef struct _qcrypto_algorithm {
	char						name[QCRYPTO_ALG_NAME_MAX];	/*!< The algorithm name. */
	qcrypto_algorithm_type_t	type;						/*!< The algorithm type. */
	union {													/*!< algorithm object, depends on type */
		qcrypto_digest_t		digest;						/*!< The digest object. */
		qcrypto_rng_t			rng;						/*!< The RNG object. */
		qcrypto_cipher_t		cipher;						/*!< The cipher object. */
		qcrypto_keygen_t		keygen;						/*!< The key generation object. */
		qcrypto_mac_t			mac;						/*!< The MAC object. */
		qcrypto_kdf_t			kdf;						/*!< The KDF object. */
		qcrypto_signature_t		signature;					/*!< The signature object. */
		qcrypto_x509_cert_t		x509;						/*!< X.509 object. */
	};
	qcrypto_algorithm_op_init	init;						/*!< The algorithm initialization function. */
	qcrypto_algorithm_op_uninit	uninit;						/*!< The algorithm un-initialization function. */
	void						*private;					/*!< Algorithm private storage. */
} qcrypto_algorithm_t;

/**
 * @brief Register an algorithm
 *
 * @param[in] alg The algorithm object.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int register_algorithm(const qcrypto_algorithm_t *alg);

/* X509 Helper functions */

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QCRYPTO_QCRYPTO_PLUGIN_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qcrypto/qcrypto_plugin/public/qcrypto/qcrypto_plugin.h $ $Rev: 921300 $")
#endif
