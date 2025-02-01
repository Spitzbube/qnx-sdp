#!/bin/sh
#include "inputs/definitions.inc"
#include "inputs/uids.inc"

. __IFS_PATH__/build/options

# By default set umask so that services don't accidentally create group/world writable files.
umask 022

#if ! defined(OPT_PATHTRUST_no) && defined(OPT_SECPOL_no)
#define ALLOW_PATHTRUST on -A allow,all,lock,path_trust
#else
#define ALLOW_PATHTRUST
#endif

echo "---> Starting slogger2"
STARTU_P(slogger_t,__SLOGGER_ID__) slogger2 DROPROOT_N(__SLOGGER_ID__) -P /pps/slogger2
waitfor /dev/slog

echo "---> Starting PCI Services"
#if defined (__HAS_PCI__)
STARTU_P(pci_server_t,__PCI_SERVER_ID__) pci-server --config=__IFS_PATH__/pci_server.cfg
waitfor /dev/pci
#if ! defined(OPT_SECPOL_no)
# This should not be necessary (HW-18805)
setfacl -m user:__IO_USB_ID__:rw /tmp/pci_*
#endif
#endif

#if defined(OPT_TCG_yes) || defined(OPT_TCG_cmdline)
echo "---> Starting TABRM"
START_P(tabrm_t) tabrm DROPROOT_NP(__TABRM_ID__)
waitfor /dev/tpmrm0
#endif

#if defined(OPT_QFIM_yes)
echo "---> Starting qfimd"
START_P(qfimd_t) qfimd
waitfor /dev/qfim/record
#endif

echo "---> Starting devb"
ALLOW_PATHTRUST START_P(devb_t) __DEVB_NAME__ SUBST_NP(cam user=__DEVB_ID__:__DEVB_ID__) blk cache=64M,auto=partition,vnode=2000,ncache=2000,commit=low __DEVB_OPTS__
waitfor /dev/hd0

echo "---> Mounting file systems"
mount_fs.sh

STARTU_P(devc_t,__DEVC_ID__) __DEVC_NAME__ -e __DEVC_OPTS__

STARTU_P(syslogd_t,__SYSLOGD_ID__) syslogd -f /system/etc/syslog.conf
START_P(random_t) random -t -p DROPROOT_NP(__RANDOM_ID__) -s /data/var/random/rnd-seed __RANDOM_INTERUPT_ENTROPY__ __RANDOM_OTHER_ENTROPY__
waitfor /dev/random
STARTU_P(pipe_t,__PIPE_ID__) pipe DROPROOT_N(__PIPE_ID__)
waitfor /dev/pipe
STARTU_P(devc_pty_t,__DEVC_PTY_ID__) devc-pty DROPROOT_NP(__DEVC_PTY_ID__)
STARTU_P(dumper_t,__DUMPER_ID__) dumper DROPROOT_N(__DUMPER_ID__) -d /data/var/dumper

on SUBST_P(-T console_t) -d -t __DEVC_PATH__ ksh -l
#if defined(__DEVC_PATH2__)
# Start more consoles which can be switched to using ctrl-alt-[1-4]
on SUBST_P(-T console2_t) -d -t __DEVC_PATH2__ ksh -l
on SUBST_P(-T console3_t) -d -t __DEVC_PATH3__ ksh -l
on SUBST_P(-T console4_t) -d -t __DEVC_PATH4__ ksh -l
#endif

echo "---> Starting Networking"
setconf _CS_HOSTNAME $OPT_HOSTNAME
START_P(io_pkt_t) io-pkt-v6-hc DROPROOT_NP(__IO_PKT_ID__) -d __NETDRIVER__ __IOPKT_TCPIP_OPTS__ __IOPKT_QNET_OPTS__
waitfor /dev/socket
start_net.sh
#if defined(OPT_IP_dhcp)
STARTU_P(dhclient_t,__DHCLIENT_ID__) dhclient -nw --no-pid -cf /system/etc/dhclient.conf -sf /system/xbin/_dhclient-script -lf /data/var/dhclient/leases -m __NETDEVICE__
#endif

echo "---> Starting sshd"
if [ ! -f /data/var/ssh/ssh_host_rsa_key ]; then
    ssh-keygen -q -t rsa -N '' -f /data/var/ssh/ssh_host_rsa_key
fi
if [ ! -f /data/var/ssh/ssh_host_ed25519_key ]; then
    ssh-keygen -q -t ed25519 -N '' -f /data/var/ssh/ssh_host_ed25519_key
fi
START_P(sshd_t) /system/xbin/sshd -f /system/etc/ssh/sshd_config

echo "---> Starting misc"
STARTU_P(qconn_t,__QCONN_ID__) qconn
STARTU_P(mq_t,__MQ_ID__) mq DROPROOT_N(__MQ_ID__)
STARTU_P(mqueue_t,__MQUEUE_ID__) mqueue
STARTU_P(pps_t,__PPS_ID__) pps DROPROOT_N(__PPS_ID__) -p /data/var/pps
START_P(io_audio_t) io-audio -d audiopci cap_name=defaultc,play_name=defaultp DROPROOT_NP(__IO_AUDIO_ID__)
#if defined(OPT_TCG_cmdline)
START_P(tabrmshare_t) tabrmshare DROPROOT_NP(__TABRMSHARE_ID__)
#endif


# Execute the post startup script.  This is a separate script to allow for common behavior of
# both slm and script based startup.
 __IFS_PATH__/post_startup.sh
