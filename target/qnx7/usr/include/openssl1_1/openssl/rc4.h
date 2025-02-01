/*
 * Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef HEADER_RC4_H
# define HEADER_RC4_H

# include <openssl/opensslconf.h>

# ifndef OPENSSL_NO_RC4
# include <stddef.h>
#ifdef  __cplusplus
extern "C" {
#endif

typedef struct rc4_key_st {
    RC4_INT x, y;
    RC4_INT data[256];
} RC4_KEY;

const char *RC4_options(void);
void RC4_set_key(RC4_KEY *key, int len, const unsigned char *data);
void RC4(RC4_KEY *key, size_t len, const unsigned char *indata,
         unsigned char *outdata);

# ifdef  __cplusplus
}
# endif
# endif

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://f27svn.qnx.com/svn/repos/osr/branches/7.0.0/trunk/openssl/dist/include/openssl/rc4.h $ $Rev: 712 $")
#endif
