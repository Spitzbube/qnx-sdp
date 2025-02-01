#ifndef __SRCVERSION_H_GUARD
#define __SRCVERSION_H_GUARD

#if !defined(__QNXNTO__) || !defined(__USESRCVERSION)
    # define __SRCVERSION(__id)
#elif defined(__ASM__)
    .macro __SRCVERSION id
        .section .ident,"SM",%progbits,1
        .asciz "\id"
        .previous
    .endm
    #define __SRCVERSION(__id) __SRCVERSION __id
#else
    # define __SRCVERSION(__id) \
        __asm__(".section .ident,\"SM\",%progbits,1; .asciz " #__id "; .previous");
#endif

#endif /* __SRCVERSION_H_GUARD */
