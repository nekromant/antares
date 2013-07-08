#!/bin/bash

ARD_PORT=$1
KERNEL=`uname`

if [ ${KERNEL:0:7} == "MINGW32" ]; then
OS="windows"
elif [ ${KERNEL:0:6} == "CYGWIN" ]; then
OS="cygwin"
elif [ $KERNEL == "Darwin" ]; then
OS="mac"
else
OS="linux"
fi

if [ $OS != "windows" ] && [ $OS != "cygwin" ]; then
    # no stty in windows and it doesn't work in cygwin, so we just skip it - you
    # need to run this script right after you plug in the board so it's still in
    # programmable mode.

    if ! [ -e "$ARD_PORT" ]; then
echo "No TTY device found -- exiting"; exit 2
    fi

for STTYF in 'stty --file' 'stty -f' 'stty <'; do
        $STTYF /dev/tty >/dev/null 2>/dev/null && break
done
    $STTYF $ARD_PORT hupcl
    (sleep 0.1 || sleep 1)
    $STTYF $ARD_PORT -hupcl
else
echo "NOTE: Can't reset device in Windows/Cygwin, so if the upload freezes try power cycling the device and retrying."
    echo
fi
