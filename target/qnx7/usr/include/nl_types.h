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

#ifndef __NL_TYPES_H__
#define __NL_TYPES_H__

struct  __nl_catd
{
    void    *__hash;
};

typedef struct __nl_catd    *nl_catd;
typedef int                 nl_item;

#define NL_SETD             0
#define NL_CAT_LOCALE       0

extern nl_catd  catopen(const char *__path, int __oflag);
extern char     *catgets(nl_catd __catd, int __setid, int __msgid,
                         const char *__defmsg);
extern int      catclose(nl_catd __catd);

#endif


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/catalog/public/nl_types.h $ $Rev: 828249 $")
#endif
