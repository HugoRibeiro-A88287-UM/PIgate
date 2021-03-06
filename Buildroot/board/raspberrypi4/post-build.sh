#!/bin/sh

set -u
set -e

# Add a console on tty1
if [ -e ${TARGET_DIR}/etc/inittab ]; then
    grep -qE '^tty1::' ${TARGET_DIR}/etc/inittab || \
	sed -i '/GENERIC_SERIAL/a\
tty1::respawn:/sbin/getty -L  tty1 0 vt100 # HDMI console' ${TARGET_DIR}/etc/inittab
fi

cp board/raspberrypi4/interfaces ${TARGET_DIR}/etc/network/interfaces
cp board/raspberrypi4/inittab ${TARGET_DIR}/etc/inittab
cp board/raspberrypi4/wpa_supplicant.conf ${TARGET_DIR}/etc/wpa_supplicant.conf
cp board/raspberrypi4/S70InitPIgate.sh ${TARGET_DIR}/etc/init.d/S70InitPIgate
