/*
 * $QNXLicenseC:
 * Copyright 2016-2020, QNX Software Systems. All Rights Reserved.
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
 * @file qcrypto.h
 * @brief QNX cryptography library API
 *
 * This shim library provides a consistent API to the various cryptographic primitives offered by
 * third-party libraries.
 */

#ifndef _QCRYPTO_QCRYPTO_H_INCLUDED
#define _QCRYPTO_QCRYPTO_H_INCLUDED

#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/cdefs.h>

#include <qcrypto/qcrypto_error.h>

/** @cond */
__BEGIN_DECLS
/** @endcond */

/**** Defines ****/

/**
 * @brief Library version.
 */
#define QCRYPTO_VERSION					1u

/**
 * @brief Shared object library version.
 */
#define QCRYPTO_VERSION_SO				"1.0"

/**
 * @brief Plugin version that is compatible with the main library version.
 *
 * @details The plugin version that is backwards compatible with
 * main library version
 */
#define QCRYPTO_PLUGIN_VERSION			1u

/**
 * @brief Default configuration filename.
 */
#define QCRYPTO_DEFAULT_CONFIG_FNAME	"qcrypto.conf"

/**
 * @brief Default configuration path.
 */
#define QCRYPTO_DEFAULT_CONFIG_PATH		"/etc/" QCRYPTO_DEFAULT_CONFIG_FNAME


/**
 * @brief Size of the largest digest (in bytes).
 */
#define QCRYPTO_DIGEST_MAX_SIZE			64

/**
 * @brief Size of the largest symmetric key (in bytes).
 */
#define QCRYPTO_SYMMETRIC_KEY_MAX_SIZE	64

/**
 * @brief Size of the largest IV (in bytes).
 */
#define QCRYPTO_IV_MAX_SIZE				16

/**
 * @brief Size of the largest tag (in bytes).
 */
#define QCRYPTO_TAG_MAX_SIZE			16

/**
 * @brief An opaque handle to a cryptographic algorithm
 *
 * @details This structure is returned by most @c qcrypto library algorithm functions and is
 *          used to manipulate algorithms.
 */
typedef struct _qcrypto_ctx qcrypto_ctx_t;

/**
 * @brief An opaque handle to a cryptographic key
 *
 * @details This structure is used to manipulate @c qcrypto library keys.
 */
typedef struct _qcrypto_key qcrypto_key_t;

/**
 * @defgroup CRYPTO_INIT_FLAGS Initialization flags
 *
 * Flags that control which plugins the @c qcrypto library initialization function loads and
 * binds.
 * @{
 */
/*! Load and bind the plugin only the first time it is accessed. */
#define QCRYPTO_INIT_LAZY		0x00
/*! Fully load and bind the first plugin found. */
#define QCRYPTO_INIT_FIRST		0x01
/*! Fully load and bind all plugins found. */
#define QCRYPTO_INIT_ALL		0x02
/** @} */

/**
 * @brief Initialize a library
 *
 * @details The @e qcrypto_init() function adds the plugin with a tag value that matches
 *          <a href="../../com.qnx.doc.neutrino.lib_ref/topic/p/__progname.html"> __progname</a>.
 *          When a user requests an algorithm, this entry is searched for a matching tag first,
 *          before the contents of the @c qcrypto library configuration file.
 *
 * @param[in] flags Initialization flag value (@c QCRYPTO_INIT_*).
 * @param[in] name The plugin name (optional).
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_init(unsigned flags, const char *name);

/**
 * @brief Un-initialize a library
 *
 * @details This function unloads a plugin and performs general cleanup. After it is called, no
 *          @c qcrypto library handles can be used or freed.
 */
void qcrypto_uninit(void);

/**
 * @brief Set the library configuration path
 *
 * @details This function allows the library to load a configuration file from the specified
 *          location instead of the default one.
 *
 * @param[in] cpath Configuration filepath.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_set_config_path(const char *cpath);

/**** Generics ****/

/**
 * @brief Release an algorithm context handle
 *
 * @details This function releases an algorithm context handle after the
 *          algorithm manipulation is complete.
 *
 * @param[in] ctx The algorithm context object.
 */
void qcrypto_release_ctx(qcrypto_ctx_t *ctx);

/**
 * @brief Retrieve the name of a plugin
 *
 * @param[in] ctx The algorithm context object.
 *
 * @return The name of the plugin or @c NULL if the context is invalid.
 *
 */
const char* qcrypto_plugin_name(qcrypto_ctx_t *ctx);

/**
 * @brief Print @c qcrypto library information
 *
 * @details This function prints information about loaded plugins and
 *          the algorithms that they support.
 */
void qcrypto_print_info(void);

/**
 * @brief Fixed-time memory comparison
 *
 * @details To resist timing attacks, this function compares the inputs in
 *          fixed time relative to the size.
 *
 * @param[in] v1 The first value.
 * @param[in] v2 The second value.
 * @param[in] size The number of bytes to compare.
 *
 * @return A value of @c 1 if the values are equal, @c 0 otherwise.
 */
int qcrypto_memcmp(const uint8_t *v1, const uint8_t *v2, size_t size);

/**
 * @brief Set the cryptography safe memory to zero
 *
 * @param[out] dst The destination buffer.
 * @param[in] size The destination buffer size.
 */
void qcrypto_memclear(void *dst, size_t size);


/**** Digests Handling ****/

/**
 * @brief Request a digest
 *
 * @details This function requests the specified algorithm from the library.
 *
 * @param[in] name The algorithm name (for example, @c sha256).
 *
 * @param[in] tag The tag value that identifies the plugin in the @c qcrypto library
 *                configuration file to retrieve the algorithm from; or NULL, to use the default
 *                tag (@e __progname).
 *
 * @param[in] flags Reserved for future use.
 *
 * @param[out] ctx A context pointer that is filled with the algorithm context on success.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_digest_request(const char *name, const char *tag, unsigned flags,
		qcrypto_ctx_t **ctx);

/**
 * @brief Query whether a digest is supported
 *
 * @details This function checks whether a cryptography provider (represented by its corresponding
 *          plugin) supports the specified algorithm.
 *
 * @param[in] name The algorithm name (for example, @c sha256).
 *
 * @param[in] tag The tag value that identifies the plugin in the @c qcrypto library
 *                configuration file to check for the algorithm; or NULL, to use the default
 *                tag (@e __progname).

 * @param[in] flags Reserved for future use.
 *
 * @return @c QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_digest_supported(const char *name, const char *tag, unsigned flags);

/**
 * @brief Query the digest size
 *
 * @details The maximum digest size is @c QCRYPTO_DIGEST_MAX_SIZE.
 *
 * @param[in] ctx The algorithm context object.
 *
 * @return The digest size in bytes.
 */
size_t qcrypto_digestsize(qcrypto_ctx_t *ctx);

/**
 * @brief Initialize a digest
 *
 * @details This function initializes or resets the internal state of a digest object.
 *
 * @param[in] ctx The algorithm context object.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_digest_init(qcrypto_ctx_t *ctx);

/**
 * @brief Update the digest
 *
 * @details This function provides the digest with data.
 *
 * @param[in] ctx The algorithm context object.
 * @param[in] data The binary data to digest.
 * @param[in] size The size of the data.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_digest_update(qcrypto_ctx_t *ctx, const uint8_t *data, size_t size);

/**
 * @brief Finalize a digest
 *
 * @details This function retrieves a digest value. It writes the same number of bytes as @e
 *          qcrypto_digestsize().
 *
 * @param[in] ctx The algorithm context object.
 * @param[out] digest The digest buffer to copy the digest value to. Make sure the buffer is large
                      enough for the digest.
 * @param[in,out] size A pointer to a variable to store the size of the digest in. Specify NULL if this
 *                     value is not required.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_digest_final(qcrypto_ctx_t *ctx, uint8_t *digest, size_t *size);

/**
 * @brief Initialize, update, and finalize a digest
 *
 * @details This function performs @e qcrypto_digest_init(), @e qcrypto_digest_update(), and
 *          @e qcrypto_digest_final() in a single operation.
 *
 * @param[in] ctx The algorithm context object.
 * @param[in] data The input data buffer.
 * @param[in] size The size of the input data.
 * @param[out] digest The digest buffer to copy the digest value to. Make sure that the buffer is
                      large enough for the digest.
 * @param[in,out] dsize A pointer to a variable to store the size of the digest in. Specify NULL if
 *                      this value is not required.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_digest(qcrypto_ctx_t *ctx, const uint8_t *data, size_t size, uint8_t *digest, size_t *dsize);

/**
 * @brief Copy a digest state
 *
 * @details Copies the current, non-finalized, digest context information
 *          so that the digest operation can be continued from the last
 *          update.
 *
 * @param[in] ctx The source algorithm context object.
 * @param[out] newctx A pointer to the copied algorithm context object.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_digest_copy(qcrypto_ctx_t *ctx, qcrypto_ctx_t **newctx);

/**** Cipher Handling ****/

/**
 * @brief Cipher padding modes
 */
typedef enum {
	QCRYPTO_CIPHER_PADDING_NONE = 0,	/*!< No padding. */
	QCRYPTO_CIPHER_PADDING_ISO7816,		/*!< ISO 7816 padding. */
	QCRYPTO_CIPHER_PADDING_PKCS7,		/*!< PKCS7 padding. */
	QCRYPTO_CIPHER_PADDING_MAX,			/*!< Maximum padding range value. */
} qcrypto_cipher_padding_t;

/**
 * @brief Cipher actions
 */
typedef enum {
	QCRYPTO_CIPHER_ENCRYPT = 1,		/*!< Encrypt. */
	QCRYPTO_CIPHER_DECRYPT,			/*!< Decrypt. */
} qcrypto_cipher_action_t;

/**
 * @brief Cipher algorithm arguments
 *
 * @details The arguments are organized based on the algorithm name.
 *          Some modes of operation require extra parameters to setup
 *          the algorithm.
 *
 *          The @e private field can be used for custom plugins that
 *          implement custom algorithms that the @c qcrypto library
 *          API does not support.
 *
 *          The padding mode can only be a value other than
 *          @c QCRYPTO_CIPHER_PADDING_NONE if the cipher is a
 *          block cipher; otherwise, an error is returned.
.*
 *          For more information, see @e qcrypto_cipher_init().
 */
typedef struct _qcrypto_cipher_args {
	qcrypto_cipher_action_t			action;		/*!< The cipher action. */
	const uint8_t					*iv;		/*!< The initialization vector (IV) buffer. */
	size_t							ivsize;		/*!< The IV size. */
	qcrypto_cipher_padding_t		padding;	/*!< Padding mode. */
	union {
		union {
			struct {
				size_t	unitsize;	/*!< The unit size (usually the blocksize). Maximum is 16 MiB. */
			} xts;
			struct {
				size_t			tagsize;	/*!< The tag (or MAC) size. */
				const uint8_t	*tag;		/*!< The decryption tag. */
			} gcm;
			struct {
				size_t			tagsize;	/*!< The tag (or MAC) size. */
				const uint8_t	*tag;		/*!< The decryption tag. */
				size_t			inputsize;	/*!< The plaintext or ciphertext size. */
				size_t			aadsize;	/*!< The AAD size. */
			} ccm;
			struct {
				size_t			tagsize;	/*!< The tag (or MAC) size. */
				const uint8_t	*tag;		/*!< The decryption tag. */
			} ocb;
		} aes;
		void			*private;	/*!< Private arguments to pass for other types of ciphers. */
	};
} qcrypto_cipher_args_t;

/**
 * @brief Request a cipher
 *
 * @details This function requests the specified algorithm from @c qcrypto.
 *
 * @param[in] name The algorithm name (for example, @c aes-128-cbc).
 *
 * @param[in] tag The tag value that identifies the plugin in the @c qcrypto library
 *                configuration file to retrieve the algorithm from; or NULL, to use the default
 *                tag (@e __progname).
 *
 * @param[in] flags Reserved for future use.
 *
 * @param[out] ctx A context pointer that is filled with the algorithm context on success.
 *
 * @return @c QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_cipher_request(const char *name, const char *tag, unsigned flags,
		qcrypto_ctx_t **ctx);

/**
 * @brief Query whether a cipher is supported
 *
 * @details This function checks whether a cryptography provider (represented by its corresponding
 *          plugin) supports the specified algorithm.
 *
 * @param[in] name The algorithm name (e.g., @c aes-128-cbc).
 *
 * @param[in] tag The tag value that identifies the plugin in the @c qcrypto library
 *                configuration file to check for the algorithm; or NULL, to use the default
 *                tag (@e __progname).
 * @param[in] flags Reserved for future use.
 *
 * @return @c QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_cipher_supported(const char *name, const char *tag, unsigned flags);

/**
 * @brief Initialize a cipher
 * @details See also <a href="manual/qcrypto_cipher_args_t.xml"><tt>qcrypto_cipher_args_t</tt></a>.
 *
 * @param[in] ctx The algorithm context object.
 * @param[in] key The symmetric key.
 * @param[in] cargs The cipher arguments.
 *
 * @return @c QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_cipher_init(qcrypto_ctx_t *ctx, qcrypto_key_t *key, const qcrypto_cipher_args_t *cargs);

/**
 * @brief Encrypt a cipher
 *
 * @details This function encrypts plaintext data using the specified symmetric key.
 *          For stream ciphers (block size == 1), the @e out buffer should be at least as large as
 *          the @e in buffer .
 *          If the input is a multiple of the block size (block size > 1), the @e out
 *          buffer should be at least as large as the @e in buffer; otherwise, the
 *          @e out buffer needs to be at least one block larger.
 *
 *          The caller needs to make sure that data is padded appropriately, if required.
 *
 * @param[in] ctx The algorithm context object.
 * @param[in] in The input data.
 * @param[in] insize The input data size.
 * @param[out] out The output data.
 * @param[in, out] outsize The combined size of the output data and the encrypted data.
 *
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_cipher_encrypt(qcrypto_ctx_t *ctx, const uint8_t *in, size_t insize,
		uint8_t *out, size_t *outsize);

/**
 * @brief Decrypt a cipher
 *
 * @details This function decrypts ciphertext data using the specified symmetric key.
 *          For stream ciphers (block size == 1), the @e out buffer should be at least as large as
 *          the @e in buffer.
 *          If the input is a multiple of the block size (block size > 1), the @e out
 *          buffer should be at least as large as the @e in buffer; otherwise, the
 *          @e out buffer needs to be at least one block larger.
 *
 * @param[in] ctx The algorithm context object.
 * @param[in] in The input data.
 * @param[in] insize The input data size.
 * @param[out] out The output data.
 * @param[in,out] outsize The combined size of the output data and the decrypted data.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_cipher_decrypt(qcrypto_ctx_t *ctx, const uint8_t *in, size_t insize,
		uint8_t *out, size_t *outsize);

/**
 * @brief Provide cipher additional authenticated data (AAD)
 *
 * @details This function is used in AEAD encryption modes such as
 *          AES GCM to provide an encryption operation with AAD.
 *
 * @param[in] ctx The algorithm context object.

 * @param[in] aad The AAD data.
 * @param[in] aadsize The AAD data size.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_cipher_aad(qcrypto_ctx_t *ctx, const uint8_t *aad, size_t aadsize);

/**
 * @brief Retrieve a cipher tag
 *
 * @details This function gets the authentication tag after encryption is complete.
 *          It is only available for AEAD ciphers.
 *
 * @param[in] ctx The algorithm context object.
 * @param[out] tag The tag data.
 * @param[in,out] tagsize The combined size of the tag data and the retrieved tag.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_cipher_get_tag(qcrypto_ctx_t *ctx, uint8_t *tag, size_t *tagsize);

/**
 * @brief Finalize a cipher
 *
 * @details This function finalizes an encryption or decryption operation by
 *          providing the last piece of data, if any. In the case of AEAD
 *          cipher modes, it allows @e qcrypto_cipher_get_tag() to retrieve the
 *          authentication tag after the encryption is successfully completed.
 *
 *          If padding is used, then the size of the @c out buffer should be at
 *          least as large as the cipher block size.
 *
 *          Although it appears that this function is not required for certain
 *          algorithms, it should always be called because cryptography plugins
 *          use it for cleanup operations when cipher operations are terminated.
 * @param[in] ctx The algorithm context object.

 * @param[out] out The output data.
 * @param[in,out] outsize The combined size of the output data and the encrypted or decrypted data.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_cipher_final(qcrypto_ctx_t *ctx, uint8_t *out, size_t *outsize);

/**
 * @brief Retrieve the cipher initialization vector (IV) size
 *
 * @details The value returned is the default size that the algorithm supports. Although some cipher modes
 *          may support alternative values, in almost all cases, the default size is
 *          recommended. See the documentation for each cipher mode for additional details.
 *
 * @param[in] ctx The algorithm context object.
 *
 * @return The IV size in bytes.
 *
 */
size_t qcrypto_cipher_ivsize(qcrypto_ctx_t *ctx);

/**
 * @brief Retrieve the cipher key size
 * @details The value returned is the default size that the algorithm supports. Although some cipher
 *          modes may support alternative values, in almost all cases, the default size is
 *          recommended. See the documentation for each cipher mode for additional details.
 *
 * @param[in] ctx The algorithm context object.
 *
 * @return The key size in bytes.
 *
 */
size_t qcrypto_cipher_keysize(qcrypto_ctx_t *ctx);

/**
 * @brief Retrieve the cipher tag size
 * @details The value returned is the default size that the algorithm supports. Although some cipher
 *          modes may support alternative values, in almost all cases, the default size is
 *          recommended. See the documentation for each cipher mode for additional details.
 *
 * @param[in] ctx The algorithm context object.
 *
 * @return The tag size in bytes.
 *
 */
size_t qcrypto_cipher_tagsize(qcrypto_ctx_t *ctx);

/**
 * @brief Retrieve the cipher blocksize
 * @details Use the value that this function returns to determine if the desired cipher algorithm
 *          requires the input data to be padded.
 *
 * @param[in] ctx The algorithm context object.
 *
 * @return The cipher blocksize in bytes.
 *
 */
size_t qcrypto_cipher_blocksize(qcrypto_ctx_t *ctx);


/**** RNG Handling ****/

/**
 * @brief RNG algorithm arguments
 *
 * @details The arguments are organized based on the algorithm name.
 *          The @c private field can be used for custom plugins that implement custom algorithms
 *          that the @c qcrypto API does not support.
 *
 *          For more information, see @e qcrypto_rng_init().
 *
 */
typedef struct _qcrypto_rng_args {
	union {
		struct {
			const uint8_t		*pstr;		/*!< The DRBG (deterministic random bit generator) personalization string. */
			size_t				pstrsize;	/*!< The DRBG personalization string length. */
		} drbg;
		void			*private;	/*!< Private arguments to pass for other types of ciphers. */
	};
} qcrypto_rng_args_t;

/**
 * @brief Request an RNG algorithm
 *
 * @details This function requests the specified algorithm from the library.
 *
 * @param[in] name The algorithm name (for example, @c drbg-ctr-128).
 *
 * @param[in] tag The tag value that identifies the plugin in the @c qcrypto library
 *                configuration file to retrieve the algorithm from; or NULL, to use the default
 *                tag (@e __progname).
 *
 * @param[in] flags Reserved for future use.
 *
 * @param[out] ctx A context pointer that is filled with the algorithm context on success.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_rng_request(const char *name, const char *tag, unsigned flags,
		qcrypto_ctx_t **ctx);

/**
 * @brief Query whether an RNG is supported
 *
 * @details This function checks whether a cryptography provider (represented by its corresponding
 *          plugin) supports the specified RNG.
 *
 * @param[in] name The algorithm name (e.g., @c drbg-ctr-128).
 *
 * @param[in] tag The tag value that identifies the plugin in the @c qcrypto library
 *                configuration file to check for the algorithm; or NULL, to use the default
 *                tag (@e __progname).
 *
 * @param[in] flags Reserved for future use.
 *
 * @return @c QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_rng_supported(const char *name, const char *tag, unsigned flags);

/**
 * @brief Initialize an RNG algorithm
 * @details  See also <a href="manual/qcrypto_rng_args_t.xml"><tt>qcrypto_rng_args_t</tt></a>.
 *
 * @param[in] ctx The algorithm context object.

 * @param[in] rargs The RNG arguments.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_rng_init(qcrypto_ctx_t *ctx, const qcrypto_rng_args_t *rargs);

/**
 * @brief Seed an RNG
 *
 * @details This function seeds the PRNG (pseudorandom number generator) with an initial value.
 *
 * @param[in] ctx The algorithm context object.
 * @param[in] seed The seed data.
 * @param[in] size The seed data size.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_rng_seed(qcrypto_ctx_t *ctx, const uint8_t *seed, size_t size);

/**
 * @brief Retrieve RNG bytes
 *
 * @details This function extracts a random value from the PRNG (pseudorandom number generator).
 *
 * @param[in] ctx The algorithm context object.
 * @param[out] buf The buffer to store the random bytes in.
 * @param[in] size The buffer size.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_rng_bytes(qcrypto_ctx_t *ctx, uint8_t *buf, size_t size);

/**** MAC Handling ****/

/**
 * @brief Request a MAC algorithm
 *
 * @details This function requests a particular algorithm from the library.
 *
 * @param[in] name The algorithm name (for example, @c hmac-sha256).
 *
 * @param[in] tag The tag value that identifies the plugin in the @c qcrypto library
 *                configuration file to retrieve the algorithm from; or NULL, to use the default
 *                tag (@e __progname).
 *
 * @param[in] flags Reserved for future use.
 *
 * @param[out] ctx A context pointer that is filled with the algorithm context on success.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_mac_request(const char *name, const char *tag, unsigned flags,
		qcrypto_ctx_t **ctx);

/**
 * @brief Query whether a MAC is supported
 *
 * @details This function checks whether a cryptography provider (represented by its corresponding
 *          plugin) supports the specified algorithm.
 *
 * @param[in] name The algorithm name (e.g., @c hmac-sha256)
 *
 * @param[in] tag The tag value that identifies the plugin in the @c qcrypto library
 *                configuration file to check for the algorithm; or NULL, to use the default
 *                tag (@e __progname).
 *
 * @param[in] flags Reserved for future use.
 *
 * @return @c QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_mac_supported(const char *name, const char *tag, unsigned flags);

/**
 * @brief Initialize a MAC algorithm
 *
 * @param[in] ctx The algorithm context object.

 * @param[in] key The symmetric key.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_mac_init(qcrypto_ctx_t *ctx, qcrypto_key_t *key);

/**
 * @brief Update a MAC algorithm
 *
 * @param[in] ctx The algorithm context object.

 * @param[in] data The data buffer.
 * @param[in] size The data buffer size.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_mac_update(qcrypto_ctx_t *ctx, const uint8_t *data, size_t size);

/**
 * @brief Finalize a MAC algorithm
 *
 * @param[in] ctx The algorithm context object.

 * @param[out] tag The MAC tag buffer.
 * @param[in,out] size The combined size of the MAC tag buffer and the retrieved tag.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_mac_final(qcrypto_ctx_t *ctx, uint8_t *tag, size_t *size);

/**
 * @brief Retrieve the MAC key size
 *
 * @param[in] ctx The algorithm context object.
 *
 * @return The key size in bytes.
 */
size_t qcrypto_mac_keysize(qcrypto_ctx_t *ctx);

/**
 * @brief MAC tag size
 *
 * @param[in] ctx The algorithm context object.
 *
 * @return The tag size, in bytes.
 */
size_t qcrypto_mac_tagsize(qcrypto_ctx_t *ctx);

/**** KDF Handling ****/

/**
 * @brief KDF arguments
 *
 * @details Some KDF functions require additional input to use them in the algorithm.
 *          The @c private field can be used for custom plugins that implement custom algorithms
 *          that the @c qcrypto library API does not support.
 *
 *          For more information, see @e qcrypto_kdf_args_t.
 */
typedef struct _qcrypto_kdf_args {
	const uint8_t	*secret;	/*!< The secret value. */
	size_t			secretsize;	/*!< The secret value size in bytes. */
	const uint8_t	*salt;		/*!< The salt value. */
	size_t			saltsize;	/*!< The salt value size in bytes. */
	union {
		struct {
			size_t	iter;		/*!< The number of hash iterations. */
		} pbkdf2;
		struct {
			const uint8_t	*info;		/*!< Extra information. */
			size_t			infosize;	/*!< The size of the extra information, in bytes */
		} hkdf;
		void		*private;	/*!< Private arguments to pass for other KDFs. */
	};
} qcrypto_kdf_args_t;

/**
 * @brief Request a KDF algorithm
 *
 * @details This function requests the specified algorithm from the library.
 *
 * @param[in] name The algorithm name (for example, @c hkdf-sha256).
 *
 * @param[in] tag The tag value that identifies the plugin in the @c qcrypto library
 *                configuration file to retrieve the algorithm from; or NULL, to use the default
 *                tag (@e __progname).
 *
 * @param[in] flags Reserved for future use.
 *
 * @param[out] ctx A context pointer that is filled with the algorithm context on success.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_kdf_request(const char *name, const char *tag, unsigned flags,
		qcrypto_ctx_t **ctx);

/**
 * @brief Query whether a KDF is supported
 *
 * @details This function checks whether a cryptography provider (represented by its corresponding
 *          plugin) supports the specified KDF.
 *
 * @param[in] name The algorithm name (e.g., @c hkdf-sha256).
 *
 * @param[in] tag The tag value that identifies the plugin in the @c qcrypto library
 *                configuration file to check for the KDF; or NULL, to use the default
 *                tag (@e __progname).
 *
 * @param[in] flags Reserved for future use.
 *
 * @return @c QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_kdf_supported(const char *name, const char *tag, unsigned flags);

/**
 * @brief Generate a symmetric key via KDF
 *
 * @details This function generates a symmetric key based on the specified input data. See also 
 *          <a href="manual/qcrypto_cipher_args_t.xml"><tt>qcrypto_kdf_args_t</tt></a>.
 *
 * @param[in] ctx The algorithm context object.
 * @param[out] key The generated key handle.
 * @param[in] keysize The key size to generate.
 * @param[in] kdfargs The KDF parameters.
 * @param[out] keyctx The algorithm context object that manipulates the resulting key, if required (optional).
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_kdf_generate(qcrypto_ctx_t *ctx, qcrypto_key_t **key, size_t keysize,
		const qcrypto_kdf_args_t *kdfargs, qcrypto_ctx_t **keyctx);

/**** Signature Handling ****/

/**
 * @brief Signature operation types
 */
typedef enum {
	QCRYPTO_SIGNATURE_SIGN = 1,		/*!< Signing. */
	QCRYPTO_SIGNATURE_VERIFY,		/*!< Verification. */
} qcrypto_signature_mode_t;

/**
 * @brief RSA signing padding modes
 */
typedef enum {
	QCRYPTO_RSA_PAD_PKCS1v15 = 0,	/*!< PKCS1 v1.5. */
	QCRYPTO_RSA_PAD_PSS,			/*!< PSS. */
} qcrypto_rsa_pad_mode_t;

/*
 *
 */
/**
 * @defgroup CRYPTO_RSA_PSS_SALT RSA PSS salt length special values
 *
 * @{
 */
/*! Salt length equals digest algorithm length (recommended). */
#define QCRYPTO_RSA_PSS_SALT_DIGEST_SIZE	SIZE_MAX-1
/*! Maximum salt length. */
#define QCRYPTO_RSA_PSS_SALT_MAX_SIZE		SIZE_MAX-2
/** @} */

/**
 * @brief Signature arguments
 *
 * @details Some signature modes require additional arguments as input
 *          to configure the algorithm.
 *          The @c private field can be used for custom plugins that implement custom algorithms
 *          that the @c qcrypto library API does not support.
 *
 *          For more information, see @e qcrypto_signature_init().
 */
typedef struct _qcrypto_signature_args {
	qcrypto_signature_mode_t	mode;	/*!< The signature operation type. */
	union {
		struct {
			qcrypto_rsa_pad_mode_t	pad_mode;	/*!< The padding mode. */
			size_t					saltsize;	/*!< The PSS salt size. */
		} rsa;
		void						*private;	/*!< Private arguments to pass for other signature algorithms. */
	};
} qcrypto_signature_args_t;

/**
 * @brief Request a signature
 *
 * @details This function requests the specified algorithm from the library.
 *
 * @param[in] name The algorithm name (for example, @c rsa-with-sha256).
 *
 * @param[in] tag The tag value that identifies the plugin in the @c qcrypto library
 *                configuration file to retrieve the algorithm from; or NULL, to use the default
 *                tag (@e __progname).
 *
 * @param[in] flags Reserved for future use.
 *
 * @param[out] ctx A context pointer that is filled with the algorithm context on success.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_signature_request(const char *name, const char *tag, unsigned flags,
		qcrypto_ctx_t **ctx);

/**
 * @brief Query whether a signature is supported
 *
 * @details This function checks whether a cryptography provider (represented by its corresponding
 *          plugin) supports the specified signature.
 *
 * @param[in] name The algorithm name (e.g., @c rsa-with-sha256)
 *
 * @param[in] tag The tag value that identifies the plugin in the @c qcrypto library
 *                configuration file to check for the algorithm; or NULL, to use the default
 *                tag (@e __progname).
 *
 * @param[in] flags Reserved for future use.
 *
 * @return @c QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_signature_supported(const char *name, const char *tag, unsigned flags);

/**
 * @brief Initialize a signature
 *
 * @details This function initializes the signature algorithm. See also
 *          <a href="manual/qcrypto_signature_args_t.xml"><tt>qcrypto_signature_args_t</tt></a>.
 *
 * @param[in] ctx The algorithm context object.
 * @param[in] key The private key that signs the signature or public key that verifies it.
 * @param[in] sargs The signature arguments.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_signature_init(qcrypto_ctx_t *ctx, qcrypto_key_t *key,
		const qcrypto_signature_args_t *sargs);

/**
 * @brief Update a signature digest
 *
 * @details This function digests the input data before a signature is generated or signed. It can
 *          be called any number of times.
 *
 * @param[in] ctx The algorithm context object.

 * @param[in] data The data buffer.
 * @param[in] size The data buffer size.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_signature_update(qcrypto_ctx_t *ctx, const uint8_t *data, size_t size);

/**
 * @brief Sign a signature
 *
 * @details This function signs the input data digest using the specified
 *          algorithm, its arguments, and a private key. Use @e
 *          qcrypto_signature_sigsize() to get the signature size and make sure that the
 *          caller can allocate a buffer that is large enough.
 *
 * @param[in] ctx The algorithm context object.
 * @param[out] sig The signature buffer.
 * @param[in,out] sigsize The combined size of the signature buffer and the retrieved signature.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_signature_sign(qcrypto_ctx_t *ctx, uint8_t *sig, size_t *sigsize);

/**
 * @brief Sign a signature using a single operation
 *
 * @details This function combines @e qcrypto_signature_update()
 *          and @e qcrypto_signature_sign() in one operation for cases where
 *          the data size is known in advance, it is small enough that it can
 *          be passed in a single operation, or both.
 *
 * @param[in] ctx The algorithm context object.
 * @param[in] data The input data.
 * @param[in] size The input data size.
 * @param[out] sig The signature buffer.
 * @param[in,out] sigsize The combined size of the signature buffer and the retrieved signature.
 *
 * @return @c QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_signature_sign_oneshot(qcrypto_ctx_t *ctx, const uint8_t *data, size_t size,
		uint8_t *sig, size_t *sigsize);

/**
 * @brief Sign a signature digest
 *
 * @details This function signs the input digest directly. It is used if the digest
 *          was calculated externally by another means and replaces @e
 *          qcrypto_signature_update() and @e qcrypto_signature_sign().
 *
 * @param[in] ctx The algorithm context object.
 * @param[in] digest The input digest.
 * @param[in] size The input digest size.
 * @param[out] sig The signature buffer.
 * @param[in,out] sigsize The combined size of the signature buffer and the retrieved signature.
 *
 * @return @c QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_signature_sign_nodgst(qcrypto_ctx_t *ctx, const uint8_t *digest, size_t size,
		uint8_t *sig, size_t *sigsize);

/**
 * @brief Verify a signature
 *
 * @details This function verifies the input data digest using the specified
 *          algorithm, its arguments, and a public key. The result is set in
 *          @c status where @c 1 means the signature was verified successfully
 *          and @c 0 means that the verification failed.
 *
 * @param[in] ctx The algorithm context object.

 * @param[in] sig The signature buffer.
 * @param[in] sigsize The signature buffer size.
 * @param[out] status The verification result.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_signature_verify(qcrypto_ctx_t *ctx, const uint8_t *sig, size_t sigsize, int *status);

/**
 * @brief Verify a signature in a single operation
 *
 * @details This function combines @e qcrypto_signature_update()
 *          and @e qcrypto_signature_verify() in one operation for cases
 *          where the data size is known in advance, is small enough
 *          that it can be passed in a single operation, or both.
 *
 * @param[in] ctx The algorithm context object.
 * @param[in] data The input data.
 * @param[in] size The input data size.
 * @param[in] sig The signature buffer.
 * @param[in] sigsize The signature buffer size.
 * @param[out] status The verification result.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_signature_verify_oneshot(qcrypto_ctx_t *ctx, const uint8_t *data, size_t size,
		const uint8_t *sig, size_t sigsize, int *status);

/**
 * @brief Verify a signature digest
 *
 * @details This function verifies the input digest. It is used if the
 *          digest was calculated externally by another means and replaces
 *          @e qcrypto_signature_update() and @e qcrypto_signature_verify().
 *
 * @param[in] ctx The algorithm context object.
 * @param[in] digest The input digest.
 * @param[in] size The input digest size.
 * @param[in] sig The signature buffer.
 * @param[in] sigsize The signature buffer size.
 * @param[out] status The verification result.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_signature_verify_nodgst(qcrypto_ctx_t *ctx, const uint8_t *digest, size_t size,
		const uint8_t *sig, size_t sigsize, int *status);

/**
 * @brief Retrieve the signature size
 *
 * @param[in] ctx The algorithm context object.

 *
 * @details This function uses the specified algorithm, its arguments,
 *          and private key information to give the caller the size of
 *          the signature that will be generated. It allows you to allocate a buffer that
 *          is large enough because you know the size of the signature in advance.
 *          Although the actual signature size can be different than what this function reports,
 *          the reported value is always large enough to contain the full signature of the specific
 *          algorithm. The following functions return the correct signature
 *          length: @e qcrypto_signature_sign(), @e qcrypto_signature_sign_oneshot(), or @e
 *          qcrypto_signature_sign_nodgst().
 *
 * @return The signature size in bytes.
 */
size_t qcrypto_signature_sigsize(qcrypto_ctx_t *ctx);

/**** Padding Handling ****/

/**
 * @brief Apply PKCS7 padding
 *
 * @details This function pads data using the specified algorithm.
 *          The size of @c paddata needs to include the correct amount of space
 *          for the padding. See @e qcrypto_padding_pkcs7_size()
 *          to calculate the required amount.
 *
 * @param[in] data The data buffer to pad (optional). Can overlap with @c paddata. Omit this
 *                 parameter to retrieve the padding bytes only, using @c size.
 * @param[in] size The data buffer size.
 * @param[out] paddata The padded data buffer.
 * @param[out] padsize The combined size of the padded data buffer and the padded data.
 * @param[in] blocksize The algorithm blocksize for padding alignment. Maximum is 255 bytes.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_padding_pkcs7_pad(const uint8_t *data, size_t size, uint8_t *paddata,
		size_t *padsize, size_t blocksize);

/**
 * @brief Remove PKCS7 padding
 *
 * @details This function verifies the padding integrity and removes it
 *          from the input data by setting the unpadded length to @c
 *          size on success.
 *
 * @param[in] data The data buffer.
 * @param[in,out] size The combined size of the data buffer and the unpadded data.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_padding_pkcs7_unpad(uint8_t *data, size_t *size);

/**
 * @brief Calculate PKCS7 padding length
 *
 * @param[in] size The input data size.
 * @param[in] blocksize The algorithm blocksize to adjust the padding to.
 *
 * @return The padding size in bytes.
 */
size_t qcrypto_padding_pkcs7_size(size_t size, size_t blocksize);

/**
 * @brief Apply ISO 7816 padding
 *
 * @details This function pads the data using the specified algorithm.
 *          The size of @c paddata needs to include the correct amount of space
 *          for the padding. See qcrypto_padding_iso7816_size()
 *          to calculate the required amount.
 *
 * @param[in] data The data buffer to pad (optional). Can overlap with @c paddata. Omit this
 *                 parameter to retrieve the padding bytes only, using @c size.
 * @param[in] size The data buffer size.
 * @param[out] paddata The padded data buffer.
 * @param[out] padsize The combined size of the padded data buffer and the padded data.
 * @param[in] blocksize The algorithm blocksize for padding alignment.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_padding_iso7816_pad(const uint8_t *data, size_t size, uint8_t *paddata,
		size_t *padsize, size_t blocksize);

/**
 * @brief Remove ISO 7816 padding
 *
 * @details This function verifies the padding integrity and removes it
 *          from the input data by setting the unpadded length to @c
 *          size on success.
 *
 * @param[in] data The data buffer.
 * @param[in,out] size The combined size of the data buffer and the unpadded data.
 *
 * @return QCRYPTO_R_EOK if successful or @c qcrypto_errno if an error occurred.
 */
int qcrypto_padding_iso7816_unpad(uint8_t *data, size_t *size);

/**
 * @brief Calculate ISO 7816 padding length
 *
 * @param[in] size The input data size.
 * @param[in] blocksize The algorithm blocksize to adjust the padding to.
 *
 * @return The padding size in bytes.
 */
size_t qcrypto_padding_iso7816_size(size_t size, size_t blocksize);

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QCRYPTO_QCRYPTO_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qcrypto/qcrypto/public/qcrypto/qcrypto.h $ $Rev: 921300 $")
#endif
