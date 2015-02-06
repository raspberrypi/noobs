#!/bin/bash

# Bash script to rebuild recovery

set -e

# Final directory where NOOBS files will be copied to
NOOBS_OUTPUT_DIR="output"

cd buildroot

BUILDROOT_DL_DIR=${BUILDROOT_DL_DIR:-"dl"}

# WARNING: don't try changing these - you'll break buildroot
BUILD_DIR="output/build"
IMAGES_DIR="output/images"

# Delete buildroot build directory to force rebuild
if [ -e "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR/"recovery* || true
fi

for i in $*; do
    # Redownload firmware from raspberrypi/firmware master HEAD to update to latest
    if [ $i = "update-firmware" ]; then
        rm -rf "$BUILD_DIR/rpi-firmware-master"
        rm -rf "$BUILDROOT_DL_DIR"/rpi-firmware-master.tar.gz
    fi

    # Redownload userland from raspberrypi/userland master HEAD to update to latest
    if [ $i = "update-userland" ]; then
        rm -rf "$BUILD_DIR/rpi-userland-master"
        rm -rf "$BUILDROOT_DL_DIR"/rpi-userland-master.tar.gz
    fi
done

# Let buildroot build everything
make

# Create output dir and copy files
FINAL_OUTPUT_DIR="../$NOOBS_OUTPUT_DIR"
mkdir -p "$FINAL_OUTPUT_DIR"
mkdir -p "$FINAL_OUTPUT_DIR/os"
cp -r ../sdcontent/* "$FINAL_OUTPUT_DIR"
cp "$IMAGES_DIR/zImage" "$FINAL_OUTPUT_DIR/recovery.img"
cp "$IMAGES_DIR/rootfs.cpio.lzo" "$FINAL_OUTPUT_DIR/recovery.rfs"

# Ensure that final output dir contains files necessary to boot
cp "$IMAGES_DIR/rpi-firmware/start_cd.elf" "$FINAL_OUTPUT_DIR/recovery.elf"
cp "$IMAGES_DIR/rpi-firmware/bootcode.bin" "$FINAL_OUTPUT_DIR"
cp "$IMAGES_DIR/cmdline.txt" "$FINAL_OUTPUT_DIR/recovery.cmdline"
touch "$FINAL_OUTPUT_DIR/RECOVERY_FILES_DO_NOT_EDIT"

# Create build-date timestamp file containing Git HEAD info for build
BUILD_INFO="$FINAL_OUTPUT_DIR/BUILD-DATA"
echo "Build-date: $(date +"%Y-%m-%d")" > "$BUILD_INFO"
echo "NOOBS Version: $(git describe)" >> "$BUILD_INFO"
echo "NOOBS Git HEAD @ $(git rev-parse --verify HEAD)" >> "$BUILD_INFO"
cat "$BUILDROOT_DL_DIR"/rpi-userland-head.version >> "$BUILD_INFO"
cat "$BUILDROOT_DL_DIR"/rpi-firmware-head.version >> "$BUILD_INFO"

cd ..

clear
echo "Build complete. Copy files in '$NOOBS_OUTPUT_DIR' directory onto a clean FAT formatted SD card to use."
