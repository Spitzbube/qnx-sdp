#ifndef __ASM_H_INCLUDED__
#define __ASM_H_INCLUDED__

#define CLABEL(_x) \
.globl _x; \
_x:;\


#define STARTFUNC(_x) \
.globl  _x; \
.type   _x,@function; \
_x:; \
.cfi_startproc; \

#define ENDFUNC(_x) \
.cfi_endproc; \
.size   _x,.-_x; \


#define STARTDATA(_x) \
.globl _x; \
.type  _x,@object; \
_x:; \

#define ENDDATA(_x) \
.size  _x, . -_x; \


#ifndef __ASM__
#define __ASM__ 1
#endif

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
#ifdef __ASM__
__SRCVERSION "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/c/public/x86_64/asm.h $ $Rev: 870901 $"
#else
__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/c/public/x86_64/asm.h $ $Rev: 870901 $" )
#endif
#endif

