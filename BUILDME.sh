#!/bin/bash

# Bash script to rebuild recovery

set -e

# Final directory where NOOBS files will be copied to
NOOBS_OUTPUT_DIR="output"


function get_package_version {
  PACKAGE=$1
  CONFIG_FILE="package/$PACKAGE/$PACKAGE.mk"
  if [ -f "$CONFIG_FILE" ]; then
    CONFIG_VAR=$(echo "$PACKAGE-version" | tr '[:lower:]-' '[:upper:]_')
    grep -E "^$CONFIG_VAR\s*=\s*.+$" "$CONFIG_FILE" | tr -d ' ' | cut -d= -f2
  fi
}


function update_github_package_version {
    PACKAGE=$1
    GITHUB_REPO=$2
    BRANCH=$3
    CONFIG_FILE="package/$PACKAGE/$PACKAGE.mk"
    if [ -f "$CONFIG_FILE" ]; then
        OLDREV=$(get_package_version $PACKAGE)
        if [ -z "$OLDREV" ]; then
            echo "Error getting OLDREV for $PACKAGE";
        else
            REPO_API=https://api.github.com/repos/$GITHUB_REPO/git/refs/heads/$BRANCH
            GITREV=$(curl -s ${REPO_API} | awk '{ if ($1 == "\"sha\":") { print substr($2, 2, 40) } }')
            if [ -z "$GITREV" ]; then
                echo "Error getting GITREV for $PACKAGE ($BRANCH)";
            else
                if [ "$OLDREV" == "$GITREV" ]; then
                    echo "Package $PACKAGE ($BRANCH) is already newest version"
                else
                    CONFIG_VAR=$(echo "$PACKAGE-version" | tr '[:lower:]-' '[:upper:]_')
                    sed -ri "s/(^$CONFIG_VAR\s*=\s*)[0-9a-f]+$/\1$GITREV/" "$CONFIG_FILE"
                    echo "Package $PACKAGE ($BRANCH) updated to version $GITREV"
                fi
            fi
        fi
    else
        echo "$CONFIG_FILE doesn't exist"
    fi
}


function get_kernel_version {
    CONFIG_FILE=.config
    CONFIG_VAR=BR2_LINUX_KERNEL_VERSION
    grep -E "^$CONFIG_VAR=\".+\"$" "$CONFIG_FILE" | tr -d '"' | cut -d= -f2
}


function update_github_kernel_version {
    PACKAGE=kernel
    GITHUB_REPO=$1
    BRANCH=$2
    CONFIG_FILE=.config
    if [ -f "$CONFIG_FILE" ]; then
        OLDREV=$(get_kernel_version)
        if [ -z "$OLDREV" ]; then
            echo "Error getting OLDREV for $PACKAGE";
        else
            REPO_API=https://api.github.com/repos/$GITHUB_REPO/git/refs/heads/$BRANCH
            GITREV=$(curl -s ${REPO_API} | awk '{ if ($1 == "\"sha\":") { print substr($2, 2, 40) } }')
            if [ -z "$GITREV" ]; then
                echo "Error getting GITREV for $PACKAGE ($BRANCH)";
            else
                if [ "$OLDREV" == "$GITREV" ]; then
                    echo "$PACKAGE ($BRANCH) is already newest version"
                else
                    CONFIG_VAR=BR2_LINUX_KERNEL_CUSTOM_REPO_VERSION
                    sed -ri "s/(^$CONFIG_VAR=\")[0-9a-f]+(\")$/\1$GITREV\2/" "$CONFIG_FILE"
                    CONFIG_VAR=BR2_LINUX_KERNEL_VERSION
                    sed -ri "s/(^$CONFIG_VAR=\")[0-9a-f]+(\")$/\1$GITREV\2/" "$CONFIG_FILE"
                    echo "$PACKAGE ($BRANCH) updated to version $GITREV"
                fi
            fi
        fi
    else
        echo "$CONFIG_FILE doesn't exist"
    fi
}


function select_kernelconfig {
    ARCH=$1
    CONFIG_FILE=.config
    CONFIG_VAR=BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE
    VAR_PREFIX=kernelconfig-recovery
    sed -ri "s/(^$CONFIG_VAR=\"$VAR_PREFIX\.).+(\")$/\1$ARCH\2/" "$CONFIG_FILE"
}


export QT_SELECT=4

cd buildroot

# WARNING: don't try changing these - you'll break buildroot
BUILD_DIR="output/build"
IMAGES_DIR="output/images"

# Delete buildroot build directory to force rebuild
if [ -e "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR/recovery-$(get_package_version recovery)" || true
fi

SKIP_KERNEL_REBUILD=0

for i in $*; do
    # Update raspberrypi/firmware master HEAD version in package/rpi-firmware/rpi-firmware.mk to latest
    if [ $i = "update-firmware" ]; then
        update_github_package_version rpi-firmware raspberrypi/firmware stable
    fi

    # Update raspberrypi/userland master HEAD version in package/rpi-userland/rpi-userland.mk to latest
    if [ $i = "update-userland" ]; then
        update_github_package_version rpi-userland raspberrypi/userland master
    fi

    # Update raspberrypi/linux rpi-4.1.y HEAD version in buildroot/.config to latest
    if [ $i = "update-kernel" ]; then
        update_github_kernel_version raspberrypi/linux rpi-4.14.y
    fi

    # Option to build just recovery without completely rebuilding both kernels
    if [ $i = "skip-kernel-rebuild" ]; then
        SKIP_KERNEL_REBUILD=1
    fi

    # Early-exit (in case we want to just update config files without doing a build)
    if [ $i = "nobuild" ]; then
        exit
    fi
done

# Let buildroot build everything
make

# Create output dir and copy files
FINAL_OUTPUT_DIR="../$NOOBS_OUTPUT_DIR"
mkdir -p "$FINAL_OUTPUT_DIR"
mkdir -p "$FINAL_OUTPUT_DIR/os"
cp -r ../sdcontent/* "$FINAL_OUTPUT_DIR"

if [ $SKIP_KERNEL_REBUILD -ne 1 ]; then
    # Rebuild kernel for ARMv7
    select_kernelconfig armv7
    make linux-reconfigure
    # copy ARMv7 kernel
    cp "$IMAGES_DIR/zImage" "$FINAL_OUTPUT_DIR/recovery7.img"

    # Rebuild kernel for ARMv6
    select_kernelconfig armv6
    make linux-reconfigure
    # copy ARMv6 kernel
    cp "$IMAGES_DIR/zImage" "$FINAL_OUTPUT_DIR/recovery.img"
else
    echo "Warning: kernels in '$NOOBS_OUTPUT_DIR' directory haven't been updated"
fi

# copy rootfs
cp "$IMAGES_DIR/rootfs.squashfs" "$FINAL_OUTPUT_DIR/recovery.rfs"
#cp "$IMAGES_DIR/rootfs.cpio.lzo" "$FINAL_OUTPUT_DIR/recovery.rfs"

# Ensure that final output dir contains files necessary to boot
cp "$IMAGES_DIR/rpi-firmware/start.elf" "$FINAL_OUTPUT_DIR/recovery.elf"
cp "$IMAGES_DIR/rpi-firmware/bootcode.bin" "$FINAL_OUTPUT_DIR"
cp -a $IMAGES_DIR/*.dtb "$IMAGES_DIR/overlays" "$FINAL_OUTPUT_DIR"
cp "$IMAGES_DIR/cmdline.txt" "$FINAL_OUTPUT_DIR/recovery.cmdline"
touch "$FINAL_OUTPUT_DIR/RECOVERY_FILES_DO_NOT_EDIT"

# Create build-date timestamp file containing Git HEAD info for build
BUILD_INFO="$FINAL_OUTPUT_DIR/BUILD-DATA"
echo "Build-date: $(date +"%Y-%m-%d")" > "$BUILD_INFO"
echo "NOOBS Version: $(sed -n 's|.*VERSION_NUMBER.*\"\(.*\)\"|v\1|p' ../recovery/config.h)" >> "$BUILD_INFO"
echo "NOOBS Git HEAD @ $(git rev-parse --verify HEAD)" >> "$BUILD_INFO"
echo "rpi-userland Git master @ $(get_package_version rpi-userland)" >> "$BUILD_INFO"
echo "rpi-firmware Git master @ $(get_package_version rpi-firmware)" >> "$BUILD_INFO"
echo "rpi-linux Git rpi-4.14.y @ $(get_kernel_version)" >> "$BUILD_INFO"

cd ..

clear
echo "Build complete. Copy files in '$NOOBS_OUTPUT_DIR' directory onto a clean FAT formatted SD card to use."
