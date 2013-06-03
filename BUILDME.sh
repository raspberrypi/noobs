#!/bin/sh

# Bash script to rebuild recovery

set -e

cd buildroot

# Delete buildroot build directory to force rebuild
if [ -e output/build ]; then
	rm -rf output/build/recovery* || true
fi

# Let buildroot build everything
make

# Copy recovery kernel and rootfs to output dir
cp output/images/zImage ../output/recovery.img
cp output/images/rootfs.cpio.lzo ../output/recovery.rfs

# Ensure that output dir contains files necessary to boot
if [ ! -e ../output/bootcode.bin ]; then
	cp output/images/rpi-firmware/start_cd.elf ../output/recovery.elf
	cp output/images/rpi-firmware/bootcode.bin ../output
fi

# Add build-date timestamp to files$

rm ../output/BUILT* || true
touch ../output/"BUILT-"$(date +"%m-%d-%Y")

cd ..

clear
echo Build complete. Copy files in \'output\' directory onto a clean FAT formatted SD card to use.
