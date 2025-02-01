--start-strip       # Avoid all the blank lines appearing in the output
#include "output/inc/options.inc"

#if defined(OPT_UNION_no)
#define __IFS_PATH__ /ifs
#else
#define __IFS_PATH__ /proc/boot
#endif

#if defined(OPT_ASLR_yes)
#define __ASLR_OPTION__ -mr
#else
#define __ASLR_OPTION__ -m~r
#endif

#define __IOPKT_TCPIP_OPTS__
#define __IOPKT_QNET_OPTS__

#if defined(OPT_QVM_yes)
#define __MODULE_QVM__ module=qvm__SAFETY__
#else
#define __MODULE_QVM__
#endif

#if defined(OPT_SECURE_PROCFS_yes)
#define __SEC_PROCFS_OPTION__ -d 0777 -u 0777
#else
#define __SEC_PROCFS_OPTION__ -d 022 -u 066
#endif

#if defined(OPT_ABLELOCK_yes)
#define __ABLE_OPTIONS__ -bl
#else
#define __ABLE_OPTIONS__
#endif

#define __PROCNTO_OPTIONS__ -v __ASLR_OPTION__ __SEC_PROCFS_OPTION__ __ABLE_OPTIONS__

#if defined(OPT_SAFE_yes)
#define __PROCNTO__ procnto-smp-instr-safety
#else
#define __PROCNTO__ procnto-smp-instr
#endif

#define __PROC_PATH__ __IFS_PATH__:/system/xbin
#if defined(OPT_GRAPHICS_yes)
#define __PROC_LIB__ __IFS_PATH__:/system/lib:/system/lib/dll:/system/graphics/vmware
#else
#define __PROC_LIB__ __IFS_PATH__:/system/lib:/system/lib/dll
#endif

#define __DATA_MOUNT_PT__ /data
#define __DATA_PREFIX__

#if ! defined(OPT_SECPOL_no) || defined(OPT_ROOT_no)
#define SUBST_NP(__SUBST__) __SUBST__
#define DROPROOT_NP(__ID__) -U __ID__:__ID__
#else
#define SUBST_NP(__SUBST__)
#define DROPROOT_NP(user)
#endif
#if ! defined(OPT_SECPOL_no)
#define SUBST_P(__SUBST__) __SUBST__
#define DROPROOT_P(__ID__) -U __ID__:__ID__
#else
#define SUBST_P(__SUBST__)
#define DROPROOT_P(__ID__)
#endif
#if defined(OPT_SECPOL_no) && defined(OPT_ROOT_no)
#define SUBST_N(__SUBST__) __SUBST__
#define DROPROOT_N(__ID__) -U __ID__:__ID__
#else
#define SUBST_N(sub)
#define DROPROOT_N(__ID__)
#endif

#define START_P(__TYPE__) SUBST_P(on -T __TYPE__)
#define STARTU_P(__TYPE__,__USER__) SUBST_P(on -T __TYPE__ -u __USER__:__USER__)
#define SLMTYPE_P(__TYPE__) SUBST_P(<SLM:type>__TYPE__</SLM:type>)
#define SLMUSER_P(__ID__)  SUBST_P(<SLM:user>__ID__:__ID__</SLM:user>)

#include "output/inc/_definitions.inc"

#if ! defined(__RANDOM_INTERUPT_ENTROPY__)
#define __RANDOM_INTERUPT_ENTROPY__
#endif

#if ! defined(__RANDOM_OTHER_ENTROPY__)
#define __RANDOM_OTHER_ENTROPY__
#endif

--end-strip
