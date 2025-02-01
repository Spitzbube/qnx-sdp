#!/bin/sh


declare -i SDP_VER
#----------------------------------------------------------------------------------------#
# The following parameters are board and BCM chipset dependent                           #
#----------------------------------------------------------------------------------------#
# WiFi chip driver name
DRV_NAME="/lib/dll/devnp-qwdi-2.5_bcm4359.so"
# WiFi driver qwdi_clm_path
DRV_CLM="/etc/firmware/4359.default.clm_blob"
# WiFi chip firmware name
FW_NAME="/etc/firmware/firmware.bcm4359.bcm.default.bin"
# WiFi chip calibration data file name
NVRAM_NAME="/etc/firmware/nvram-murata-bcm989359_Type1FD_160307.txt"
# QNX SDP version (6 or 7)
SDP_VER=7

#---------------------End board and BCM chipset dependent section------------------------#

echo "\n"
echo "PLEASE CHECK THE FOLLOWING - UPDATE THE SCRIPT $0 AS NEEDED"
echo "==================================================================================="
echo "Insure that these parameter are correct for your board"
echo "==================================================================================="
echo "DRV_NAME=$DRV_NAME"
echo "DRV_CLM=$DRV_CLM"
echo "FW_NAME=$FW_NAME"
echo "NVRAM_NAME=$NVRAM_NAME"
echo "SDP_VER=$SDP_VER"
echo "==================================================================================="
echo "\n"


# path and names for supplicant, conf, hostapd
HAPD_NAME="hostapd-2.5"
HOST_APD_CMD="/usr/sbin/$HAPD_NAME"
SUPP_NAME="/usr/sbin/wpa_supplicant-2.5"
SUPP_CONF="/etc/wpa_supplicant-2.5.conf"
WL_CMD="/usr/sbin/wl-bcm4359"
WPA_PPS_CONF_DIR="/var/etc/wpa_pps.conf"

# debug options
SUPP_LOG="/dev/null"
IO_PKT_INSTANCE=""
SUPP_VERBOSITY="-q" # "-dd" for debugging
WPA_DEBUG=""    # "-d" for debugging

# startup options
P_FORMAT=""
SOKET_PATH=""
START_WIFI_P2P=0
START_WIFI_AP=0
STOP_WIFI=0




#----------------------------------------------------------------------------------------#
# Interface assighments are dynamic depending on the script startup options              #
# Values will be assigned by the scripts based on startup options                        #
# AP + STA: bcm1 is AP , bcm0 is STA                                                     #
# STA+P2P:  bcm0 is STA, bcm_p2pdev0 is p2p_dev, bcm2 is p2p_grp (for SDP660 and lower)  #
# STA+P2P:  bcm0 is STA, bcm_p2pdev0 is p2p_dev, bcm1 is p2p_grp (for SDP7 and higher)   #
#----------------------------------------------------------------------------------------#
STA_IF=""
AP_IF=""
P2P_DEV=""
P2P_GRP=""




help()
{
    echo "\n"
    echo "use one of the options in below: "
    echo "    -p: STA-P2p operation, mount wifi driver, start supplicant, spawn wpa_pps"
    echo "    -a: STA-AP operation, mount wifi driver, start supplicant, spawn wpa_pps"
    echo "    -d: Stop WiFi, unmount wifi driver, stop supplicant, wpa_pps and tetherman and destroy wifi interfaces"
    echo "    -i: io-pkt instance number on which to mount the wifi driver (defaults to no instance in mount command)"
    echo "    -l: supplicant log folder (defaults to /dev/null) e.g. /mnt/log/supplicant.txt"
    echo "    -v  supplicant log verbosity level ( e.g. -v d to or -v dd or -v ddd (add more d's to increase)"
    echo "    -x  wpa_pps in P-formatted ssid operation"
    echo "    -s  specify the path to the supplicant socket if other than default"
    echo "Usage: ${0##*/} [-p|-a][-d][-i <io_pkt inst>][-l <supplicant_log_file>][-v supplicant__verbosity>] [-x] [-s supplicant_socket path]"
    exit
}


destroyIF()
{
    echo "==>destroy ifaces if exists"
    for iface in bcm1: bcm0: bcm2: bcm_p2pdev0:
    do
        ifName=$(ifconfig | grep $iface | grep -v grep | awk '{print $1}')

        if [[ $iface == $ifName ]]; then
            echo "--> destroy ${iface%?}"
            ifconfig ${iface%?} destroy
        else
            echo -n "."
        fi

    done
    ifconfig
}


slayDhcp()
{
    # Remove the last character, e. g. bcm2 changes to bcm.
    # the last character should be removed otherwise it wont be found to delete.
    tmpAP_IF=$(echo $AP_IF | sed 's/.$//')

    for procName in dhcpd
    do
        echo "\n==>Look for $procName ($tmpAP_IF) to slay"
        pid=$(pidin arg | grep $procName | grep -v grep | grep $tmpAP_IF | awk '{print $1}')
        if [ -z "$pid" ]; then
            echo -n "."
        else
            echo "--> slay ${procName}"
            slay $pid
        fi
    done
}

slayDhclient()
{
    # Remove the last character, e. g. bcm2 changes to bcm.
    # the last character should be removed otherwise it wont be found to delete.
    tmpSTA_IF=$(echo $STA_IF | sed 's/.$//')
    echo "\n==>Look for dhclient ($tmpSTA_IF) to release"
    NOT_FOUND=0
    while [ $NOT_FOUND -eq 0 ]
    do
        typeset pidFile=$(pidin arg | grep dhclient | grep $tmpSTA_IF | sed -n 's/.\+-pf \([^ ]\+\).*/\1/p')

        if [ -z ${pidFile} ]; then
            echo -n "."
            NOT_FOUND=1
        else
            echo "--> ${pidFile}"
            dhclient -pf ${pidFile} -r
        fi
    done

    echo ""
}

slayProcess()
{
    echo "==>slay specific processes if running"
    for procName in wpa_pps tetherman $SUPP_NAME $HAPD_NAME
    do
        echo "\n==>Look for $procName to slay"
        pid=$(pidin | grep $procName | grep -v grep | awk '{print $1}')
        if [ -z "$pid" ]; then
            echo -n "."
        else
            echo "--> slay ${procName}"
            slay $pid
        fi
    done

    # Slay dhcp*
    slayDhcp

    # Slay dhclient
    slayDhclient
}


mountDrv()
{
    echo "==>mount wifi driver $DRV_NAME"
    if [ -e /dev/${STA_IF}_ctrl ] ; then
        echo "\n -------> driver already mounted\n"
        echo "\n FAILED \n"
        exit 1
    fi

    connectPCI

    if [ -e $FW_NAME ] && [ -e $NVRAM_NAME ] && [ -e $DRV_NAME ] ; then
        echo "mount -Tio-pkt$IO_PKT_INSTANCE -o nvram=${NVRAM_NAME},fw=${FW_NAME},qwdi_clm_path=${DRV_CLM},qwdi_dbg_level=0,dhd_msg_level=0x1 $DRV_NAME"
        mount -Tio-pkt$IO_PKT_INSTANCE -o nvram=${NVRAM_NAME},fw=${FW_NAME},qwdi_clm_path=${DRV_CLM},qwdi_dbg_level=0,dhd_msg_level=0x1 $DRV_NAME
        echo "\n ---> waitfor /dev/bcm_p2pdev0 3"
        waitfor /dev/bcm_p2pdev0_ctrl 3
        if [ -e /dev/bcm_p2pdev0_ctrl ] && [ -e /dev/bcm0_ctrl ] && [ -e /dev/bcm1_ctrl ] && [ -e /dev/bcm2_ctrl ] ; then
            echo "\n -----------------> DRIVER SUCCESS <--------------------\n"
        else
            echo "\n -----------------> DRIVER FAILED  <--------------------\n"
            echo "\n FAILED \n"
            exit 1
        fi
    else
        echo "\n *************************************"
        echo "\n ---> Error, driver files are missing $FW_NAME or $NVRAM_NAME or $DRV_NAME "
        ls $FW_NAME
        ls $NVRAM_NAME
        ls $DRV_NAME
        echo "\n *************************************"
        echo "\n FAILED \n"
        exit 1
    fi

    # long delay is needed before sending the following command
    sleep 3
    echo "==> $WL_CMD -i bcm0 rsdb_mode 0"
    $WL_CMD -i bcm0 rsdb_mode 0
    if [[  $? == 0 ]]; then
        echo "bcm0 rsdp_mode 0 is set"
    else
        echo "could not setup bcm0 rsdb_mode"
        echo "\n FAILED \n"
        exit 1
    fi


}


connectPCI()
{
    PCI_ON=$(pidin arg | grep pci | grep -v grep)

    if [[ $PCI_ON != *"pci-server"* ]]; then
        echo "==>connect pci-server"
        /base/sbin/pci-server -c &
        sleep 1
    else
            echo "==>pci-server already connected"
    fi
}

# Send the generic return result to compare with any other string and print the result
printStatus_Gen()
{
    if [[ $1 == $2 ]]; then
        echo "SUCCESS"
    else
        echo "FAILED"
    fi
}

printStatus()
{
    if [ -z $1 ]; then
        echo "FAILED"
    else
        echo "SUCCESS"
    fi
}


setup_wpa_supplicant()
{
    echo "==>setup STA mode, $STA_IF"
    if [ -e $SUPP_NAME ] && [ -e $SUPP_CONF ] && [ -e $DRV_NAME ] ; then
        echo "$SUPP_NAME -t -Dqwdi -i$STA_IF -c$SUPP_CONF -g/var/run/wpa_supplicant_global $SUPP_VERBOSITY > $SUPP_LOG&"
        $SUPP_NAME -t -Dqwdi -i$STA_IF -c$SUPP_CONF -g/var/run/wpa_supplicant_global $SUPP_VERBOSITY > $SUPP_LOG&
        echo "\n ---> waitfor /var/run/wpa_supplicant_global 5"
        waitfor /var/run/wpa_supplicant_global 5
        if [ -e /var/run/wpa_supplicant_global ] ; then
            echo "\n -----------------> SUPPLICANT SUCCESS <--------------------\n"
        else
            echo "\n -----------------> SUPPLICANT FAILED  <--------------------\n"
            echo "\n FAILED \n"
            exit 2
        fi
    else
        echo "\n *************************************"
        echo "\n ---> Error, wpa supplicant files are missing $SUPP_NAME or $SUPP_CONF or $DRV_NAME "
        ls $SUPP_NAME
        ls $SUPP_CONF
        ls $DRV_NAME
        echo "\n *************************************"
        echo "\n FAILED \n"
        exit 2
    fi

    echo "\n************************ Done ************************\n"
    sleep 3
    ifconfig

    # Check status
    typeset pid=$(pidin arg | grep wpa_supplicant | grep -v grep | awk '{print $1}')
    echo "\n -----------------> SUPPLICANT $(printStatus $pid) <--------------------\n"
}

setup_wpa_pps()
{
    echo "\n*************** setup wpa_pps and tetherman ************\n"
    if [ -e $WPA_PPS_CONF_DIR ] && [ -e $HOST_APD_CMD ] ; then
        if [ "$START_WIFI_P2P" -eq 1 ]; then
            echo "$WPA_DEBUG $P_FORMAT $SOKET_PATH  -c $WPA_PPS_CONF_DIR -f $P2P_DEV -k $P2P_GRP -v qwdi  -i $STA_IF &"
            wpa_pps $WPA_DEBUG $P_FORMAT $SOKET_PATH  -c $WPA_PPS_CONF_DIR -f $P2P_DEV -k $P2P_GRP -v qwdi  -i $STA_IF &
            waitfor /pps/services/wifi/status_p2p 5
            if [ -e /pps/services/wifi/status_p2p ]; then
                echo "Wifi P2P PPS Status SUCCESS"
            else
                echo "Wifi P2P PPS Status FAILED"
            fi
        else
           echo "wpa_pps $WPA_DEBUG $P_FORMAT $SOKET_PATH  -c $WPA_PPS_CONF_DIR -v qwdi  -j $AP_IF -h $HOST_APD_CMD -i $STA_IF &"
           wpa_pps $WPA_DEBUG $P_FORMAT $SOKET_PATH  -c $WPA_PPS_CONF_DIR -v qwdi  -j $AP_IF -h $HOST_APD_CMD -i $STA_IF &
           tetherman &
           waitfor /pps/services/wifi/status 5
           if [ -e /pps/services/wifi/status ]; then
               echo "Wifi PPS Status SUCCESS"
           else
               echo "Wifi PPS Status FAILED"
           fi
        fi
    else
        echo "\n *************************************"
        echo "\n ---> Error, wpa supplicant files are missing $HOST_APD_CMD or $WPA_PPS_CONF_DIR "
        ls $HOST_APD_CMD
        ls $WPA_PPS_CONF_DIR
        echo "\n *************************************"
        echo "\n FAILED \n"
        exit 3
    fi
    echo "\n************************ Done ************************\n"


    # get and print wpa_pps and tetherman pid
    typeset pid=$(pidin arg | grep wpa_pps | grep -v grep | awk '{print $1}')
    echo "\n -----------------> wpa_pps $(printStatus $pid) <--------------------\n"
    if [ "START_WIFI_AP" -eq 1 ]; then
        # Check tetherman status
        typeset pid=$(pidin arg | grep tetherman | grep -v grep | awk '{print $1}')
        echo "\n -----------------> tetherman $(printStatus $pid) <--------------------\n"
    fi
}




StopWiFi()
{
    # Slay the processes except for pci-server
    slayProcess
    sleep 1

    # Destroy the interfaces
    destroyIF
    sleep 1
}

######### Main starts here #########



while getopts :dpai:l:v:xs: opt; do
  case $opt in
    p)
      let START_WIFI_P2P=1
      let START_WIFI_AP=0
      echo "start wifi STA-P2P = $START_WIFI_P2P"
      STA_IF="bcm0"
      P2P_DEV="bcm_p2pdev0"
      if [[ $SDP_VER == 6 ]]; then
          P2P_GRP="bcm2"
      else
          P2P_GRP="bcm1"
      fi
      echo "interfaces are STA: $STA_IF , P2P-Dev: $P2P_DEV , P2p-GRP: $P2P_GRP"
      ;;
    a)
      let START_WIFI_P2P=0
      let START_WIFI_AP=1
      echo "start wifi STA-AP $START_WIFI_AP"
      STA_IF="bcm0"
      AP_IF="bcm1"
      echo "STA-IF: $STA_IF , AP-IF: $AP_IF"
     ;;
    d)
      let STOP_WIFI=1
      ;;
    i)
      IO_PKT_INSTANCE=$OPTARG
      ;;
    l)
      SUPP_LOG=$OPTARG
      ;;
    v)
      SUPP_VERBOSITY="-$OPTARG"
      ;;
    x)
      P_FORMAT="-x"
      ;;
    s)
      SOKET_PATH="-s $OPTARG"
      ;;
    \?)
      echo "\nInvalid option: -$OPTARG"
      help
      ;;
    :)
      echo "\nOption -$OPTARG requires an argument."
      help
      ;;
  esac
done

if [ $# -eq 0 ]; then
    help
elif [ "$STOP_WIFI" -eq 1 ];then
    echo "Stopping Wifi"
    StopWiFi
elif [ "$START_WIFI_P2P" -eq 1 ]  || [ "$START_WIFI_AP" -eq 1 ];then
    echo "Starting Wifi on IO_PKT_INSTANCE=$IO_PKT_INSTANCE with SUPP_VERBOSITY=$SUPP_VERBOSITY and logging to $SUPP_LOG"
    mountDrv
    setup_wpa_supplicant
    setup_wpa_pps
else
   echo "Error in options provided"
   help
fi

exit 0