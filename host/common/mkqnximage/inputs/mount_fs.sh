#!/bin/sh
#include "inputs/definitions.inc"

echo "---> Mounting file systems"

if [ -e /data -a -e /system ]; then
	exit 0
fi

#if ! defined(__SYS_TRUST_OPT__)
#define __SYS_TRUST_OPT__
#endif

#if defined(OPT_BOOT_MODE_uefi)
#define __BOOT_TYPE__ dos
#define __BOOT_PART__ /dev/hd0.efi.0
#if defined(OPT_QTD_yes)
#define __SYSTEM_PART__ /dev/hd0.qtd.1
#else
#define __SYSTEM_PART__ /dev/hd0.qnx6.1
#endif
#define __DATA_PART__ /dev/hd0.qnx6.2
#else
#define __BOOT_TYPE__ qnx6
#define __BOOT_PART__ /dev/hd0t177
#if defined(OPT_QTD_yes)
#define __SYSTEM_PART__ /dev/hd0t185
#else
#define __SYSTEM_PART__ /dev/hd0t178
#endif
#define __DATA_PART__ /dev/hd0t179
#endif

#if defined(OPT_QTD_yes)
# Mounting a QTD protected file system is a two step process.  First mount the QTD container
# and then the file system contained within
mount -t qtd -o key=__IFS_PATH__/qtd_public_key.pem __SYS_TRUST_OPT__ __SYSTEM_PART__ __SYSTEM_PART__-qnx6
mount -t qnx6 -o sync=optional,mntperms=755,noatime __SYS_TRUST_OPT__ __SYSTEM_PART__-qnx6 /system
#else
mount -t qnx6 -o sync=optional,mntperms=755 __SYS_TRUST_OPT__ __SYSTEM_PART__ /system
#endif

#if ! defined(OPT_SECURE_DATA_no)
#define __DATA_OPTIONS__ -o __OPT_SECURE_DATA__
#else
#define __DATA_OPTIONS__
#endif
mount -t qnx6 -o sync=optional,mntperms=755 __DATA_OPTIONS__ __DATA_PART__ __DATA_MOUNT_PT__
mount -t __BOOT_TYPE__ __BOOT_PART__ /boot

ln -sPf /data/var/tmp /tmp
/system/xbin/cleanup_tmp
