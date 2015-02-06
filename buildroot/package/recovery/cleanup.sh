#!/bin/sh

# this is relative to the 'buildroot' subdirectory
BUILDROOT_OUTPUT_DIR=${BUILDROOT_OUTPUT_DIR:-"output"}
BUILDROOT_TARGET_DIR="$BUILDROOT_OUTPUT_DIR/target"

# Remove redundant fonts
#BUILDROOT_STAGING_DIR="$BUILDROOT_OUTPUT_DIR/staging"
#STAGING_FONTS_DIR="$BUILDROOT_STAGING_DIR/usr/lib/fonts/"
#TARGET_FONTS_DIR="$BUILDROOT_TARGET_DIR/usr/lib/fonts"
#if [ -e "$TARGET_FONTS_DIR/VeraBd.ttf" ]; then
#	rm "$TARGET_FONTS_DIR/"*
#	cp "$STAGING_FONTS_DIR/DejaVuSans.ttf" "$TARGET_FONTS_DIR"
#	cp "$STAGING_FONTS_DIR/DejaVuSans-Bold.ttf" "$TARGET_FONTS_DIR"
#fi

# We do not use busybox's init system
rm -f "$BUILDROOT_TARGET_DIR/etc/init.d/"*

# We do not use Qt's tga/ico plugins
#rm -rf "$BUILDROOT_TARGET_DIR/usr/lib/qt/plugins"


