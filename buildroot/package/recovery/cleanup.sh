#!/bin/sh

# Remove redudant fonts
#if [ -e output/target/usr/lib/fonts/VeraBd.ttf ]; then
#	rm output/target/usr/lib/fonts/*
#	cp output/staging/usr/lib/fonts/DejaVuSans.ttf output/target/usr/lib/fonts
#	cp output/staging/usr/lib/fonts/DejaVuSans-Bold.ttf output/target/usr/lib/fonts
#fi

# We do not use busybox's init system
rm -f output/target/etc/init.d/*

# We do not use Qt's tga/ico plugins
#rm -rf output/target/usr/lib/qt/plugins


