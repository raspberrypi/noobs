#############################################################
#
# rpi-firmware
#
#############################################################

RPI_FIRMWARE_VERSION = master
RPI_FIRMWARE_SITE = http://github.com/raspberrypi/firmware/tarball/$(RPI_FIRMWARE_VERSION)
RPI_FIRMWARE_LICENSE = BSD-3c
RPI_FIRMWARE_LICENSE_FILES = boot/LICENCE.broadcom

# we're downloading a branchname rather than a commit-id, so this gets the commit-id we just downloaded
define RPI_FIRMWARE_GET_CURRENT_VERSION
	echo "rpi-firmware Git HEAD @ "`git ls-remote --heads https://github.com/raspberrypi/firmware | grep refs/heads/$(RPI_FIRMWARE_VERSION)` > $(BR2_DL_DIR)/rpi-firmware-head.version
endef

RPI_FIRMWARE_POST_DOWNLOAD_HOOKS += RPI_FIRMWARE_GET_CURRENT_VERSION

define RPI_FIRMWARE_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0644 $(@D)/boot/bootcode.bin $(BINARIES_DIR)/rpi-firmware/bootcode.bin
	$(INSTALL) -D -m 0644 $(@D)/boot/start.elf $(BINARIES_DIR)/rpi-firmware/start.elf
	$(INSTALL) -D -m 0644 $(@D)/boot/start_cd.elf $(BINARIES_DIR)/rpi-firmware/start_cd.elf
	$(INSTALL) -D -m 0644 $(@D)/boot/fixup.dat $(BINARIES_DIR)/rpi-firmware/fixup.dat
	$(INSTALL) -D -m 0644 $(@D)/boot/fixup_cd.dat $(BINARIES_DIR)/rpi-firmware/fixup_cd.dat
	$(INSTALL) -D -m 0644 package/rpi-firmware/config.txt $(BINARIES_DIR)/rpi-firmware/config.txt
endef

$(eval $(generic-package))
