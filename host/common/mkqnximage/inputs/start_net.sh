#!/bin/sh
#include "inputs/definitions.inc"
#include "inputs/uids.inc"

if_up -p __NETDEVICE__
ifconfig __NETDEVICE__ up
#if ! defined(OPT_IP_dhcp)
. __IFS_PATH__/build/options
ifconfig __NETDEVICE__ $OPT_IP
#endif
sysctl -w net.inet.icmp.bmcastecho=1
#if defined(OPT_ROOT_no)
sysctl -w qnx.kern.droproot=0x1
#endif

#if ! defined(OPT_SECPOL_no)
# If dhclient not run as root, need to give it read/write access to /dev/bpf0
setfacl -m user:__DHCLIENT_ID__:rw  /dev/bpf0
#endif

exit 0
