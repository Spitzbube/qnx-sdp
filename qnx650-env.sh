# This script is sets environment variables requires to use this version of QNX Software Development Platform 6.5
# from the command line. To use the script, you have to "source" it into your shell,
# source qnx650-env.sh
# if source command is not available use "." (dot) command instead
#

test "$BASH_SOURCE" = "" && echo "This script only can be run from bash" && return
SCRIPT_SOURCE=$BASH_SOURCE
test "$SCRIPT_SOURCE" = "$0" && echo "Script is being run, should be sourced" && exit 1

HOST_OS=$(uname -s)
SCRIPT_DIR=$(dirname "${SCRIPT_SOURCE}")
QNX_BASE=$(cd "${SCRIPT_DIR}"; pwd)

QNX_TARGET=$QNX_BASE/target/qnx6
QNX_HOST=$QNX_BASE/host/linux/x86
QNX_CONFIGURATION=/opt/qnx650/.qnx
MAKEFLAGS=-I$QNX_BASE/target/qnx6/usr/include
PATH=$QNX_HOST/usr/bin:/opt/qnx650/.qnx/bin:/opt/qnx650/jre/bin:$PATH

export QNX_TARGET QNX_HOST QNX_CONFIGURATION MAKEFLAGS PATH
unset PYTHONPATH

# Include CAR2.1 Environments if present
qnxCarDeployment=/opt/qnx650/deployment/qnx-car
[ -x $qnxCarDeployment/qnxcar-env.sh ] && 
   . $qnxCarDeployment/qnxcar-env.sh
