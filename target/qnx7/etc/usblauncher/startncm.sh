#!/bin/sh

# The option usbdnet_mac assigns the specified MAC address to the Host side
# The option mac assigns the specified MAC address on the device side
# Both have the locally admin bit
echo "usblauncher role switch - starting startncm.sh script (/ut_carplay/etc/usblauncher/startncm.sh)"

# use explicit path since io-pkt's library search path doesn't include /ut_carplay/armle-v7/lib/dll
echo "Mounting NCM driver"
mount -Tio-pkt -o verbose=0,protocol=ncm,usbdnet_mac=000022446688,mac=020022446688,name=${CARPLAY_IFACE_NAME} /ut_carplay/armle-v7/lib/dll/devnp-usbdnet.so
