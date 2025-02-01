#!/bin/sh
#include "inputs/definitions.inc"
#include "inputs/uids.inc"

# Configure locations of PAM files.  Note, pamconf must end with a slash
setconf pamlib /system/pam/lib
setconf pamconf /system/pam/config/

if [ ! -e /pps/slogger2 ]; then
	mkdir /pps/slogger2
fi

#include "output/inc/_post_start.inc"

pinger &

echo Process count:`pidin arg | wc -l`

exit 0
