#!/bin/bash

# Bash script to rebuild recovery

set -e

cd buildroot

# Delete buildroot build directory to force rebuild
if [ -e output/build ]; then
    rm -rf output/build/recovery* || true
fi

for i in $*; do
    # Redownload firmware from raspberrypi/firmware master HEAD to update to latest
    if [ $i = "update-firmware" ]; then
        rm -rf output/build/rpi-firmware-master
        rm -rf dl/rpi-firmware-master.tar.gz
        echo "rpi-firmware Git HEAD @ "`git ls-remote --heads https://github.com/raspberrypi/firmware | sed -n 2p` > rpi-firmware-head &
    fi

    # Redownload userland from raspberrypi/userland master HEAD to update to latest
    if [ $i = "update-userland" ]; then
        rm -rf output/build/rpi-userland-master
        rm -rf dl/rpi-userland-master.tar.gz
        echo "rpi-userland Git HEAD @ "`git ls-remote --heads https://github.com/raspberrypi/userland | sed -n 2p` > rpi-userland-head &
    fi
done

# Let buildroot build everything
make

# Copy recovery kernel and rootfs to output dir
cp output/images/zImage ../output/recovery.img
cp output/images/rootfs.cpio.lzo ../output/recovery.rfs

# Ensure that output dir contains files necessary to boot
cp output/images/rpi-firmware/start_cd.elf ../output/recovery.elf
cp output/images/rpi-firmware/bootcode.bin ../output

# Create build-date timestamp file containing Git HEAD info for build
rm -f ../output/BUILT* || true
BUILD_INFO="../output/BUILD-DATA"
echo "Build-date: $(date +"%Y-%m-%d")" > "$BUILD_INFO"
echo "NOOBS Version: "`git describe` >> "$BUILD_INFO"
echo "NOOBS Git HEAD @ "`git rev-parse --verify HEAD` >> "$BUILD_INFO"
cat rpi-userland-head >> "$BUILD_INFO"
cat rpi-firmware-head >> "$BUILD_INFO"

cd ..

clear
echo "Build complete. Copy files in 'output' directory onto a clean FAT formatted SD card to use."
