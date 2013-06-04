#!/bin/sh

# Bash script to rebuild recovery

set -e

cd buildroot

# Delete buildroot build directory to force rebuild
if [ -e output/build ]; then
    rm -rf output/build/recovery* || true
fi

# Redownload firmware from raspberrypi/firmware master HEAD to update to latest
if [ $1 = "update-firmware" ]; then
    rm -rf output/build/rpi-firmware-master
    rm -rf dl/rpi-firmware-master.tar.gz
fi

# Let buildroot build everything
make

# Copy recovery kernel and rootfs to output dir
cp output/images/zImage ../output/recovery.img
cp output/images/rootfs.cpio.lzo ../output/recovery.rfs

# Ensure that output dir contains files necessary to boot
cp output/images/rpi-firmware/start_cd.elf ../output/recovery.elf
cp output/images/rpi-firmware/bootcode.bin ../output

# Add build-date timestamp to files$

rm ../output/BUILT* || true
touch ../output/"BUILT-"$(date +"%Y-%m-%d")

cd ..

clear
echo Build complete. Copy files in \'output\' directory onto a clean FAT formatted SD card to use.
