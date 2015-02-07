#!/bin/bash

# Bash script to run linux-menuconfig for a specific arch

set -e

ARCH=$1
MAKECMD=${2:-"linux-menuconfig"}
if [ -n "$ARCH" ]; then
    if [ "$ARCH" != "armv6" ] && [ "$ARCH" != "armv7" ]; then
        echo "ARCH must be armv6 or armv7">&2
        exit 1
    fi
else
    echo "Missing ARCH parameter">&2
    exit 1
fi


function get_kernel_version {
    CONFIG_FILE=.config
    CONFIG_VAR=BR2_LINUX_KERNEL_VERSION
    grep -E "^$CONFIG_VAR=\".+\"$" "$CONFIG_FILE" | tr -d '"' | cut -d= -f2
}


function select_kernelconfig {
    ARCH=$1
    CONFIG_FILE=.config
    CONFIG_VAR=BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE
    VAR_PREFIX=kernelconfig-recovery
    sed -ri "s/(^$CONFIG_VAR=\"$VAR_PREFIX\.).+(\")$/\1$ARCH\2/" "$CONFIG_FILE"
}


function get_kernelconfig {
    CONFIG_FILE=.config
    CONFIG_VAR=BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE
    VAR_PREFIX=kernelconfig-recovery
    grep -E "^$CONFIG_VAR=\".+\"$" "$CONFIG_FILE" | tr -d '"' | cut -d= -f2 | sed "s/^$VAR_PREFIX\.//"
}


cd buildroot

# Setup kernel for specific arch
if [ "$(get_kernelconfig)" != "$ARCH" ]; then
    select_kernelconfig $ARCH
    KERNEL_BUILD_DIR=output/build/linux-$(get_kernel_version)
    rm "$KERNEL_BUILD_DIR/.stamp_patched"
    rm "$KERNEL_BUILD_DIR/.stamp_configured"
fi

# Run specific command
make "$MAKECMD"

