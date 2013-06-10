#############################################################
#
# rpi-userland
#
#############################################################

RPI_USERLAND_VERSION = 5e9a740a88a889dfc8a18bb1b00c17e5dd9d0108
RPI_USERLAND_SITE = http://github.com/raspberrypi/userland/tarball/master
RPI_USERLAND_LICENSE = BSD-3c
RPI_USERLAND_LICENSE_FILES = LICENCE
RPI_USERLAND_INSTALL_STAGING = YES
RPI_USERLAND_CONF_OPT = -DVMCS_INSTALL_PREFIX=/usr


define RPI_USERLAND_POST_TARGET_CLEANUP
    rm -Rf $(TARGET_DIR)/usr/src
endef

# Recovery edit

#RPI_USERLAND_POST_INSTALL_TARGET_HOOKS += RPI_USERLAND_POST_TARGET_CLEANUP

define RPI_USERLAND_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(STAGING_DIR)/usr/lib/libvchiq_arm.so $(TARGET_DIR)/usr/lib
        $(INSTALL) -m 0755 $(STAGING_DIR)/usr/lib/libvcos.so $(TARGET_DIR)/usr/lib
	$(INSTALL) -m 0755 $(STAGING_DIR)/usr/bin/tvservice $(TARGET_DIR)/usr/bin
	$(INSTALL) -m 0755 $(STAGING_DIR)/usr/bin/vcgencmd $(TARGET_DIR)/usr/bin
endef

$(eval $(cmake-package))
