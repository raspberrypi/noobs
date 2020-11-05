CC=$(PWD)/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabi-gcc

all: con2fbmap
	$(MAKE) $(AM_MAKEFLAGS) con2fbmap

con2fbmap: con2fbmap.c
	$(CC) -static con2fbmap.c -o con2fbmap



