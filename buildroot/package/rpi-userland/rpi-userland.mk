#############################################################
#
# rpi-userland
#
#############################################################

RPI_USERLAND_VERSION = master
RPI_USERLAND_SITE = http://github.com/raspberrypi/userland/tarball/$(RPI_USERLAND_VERSION)
RPI_USERLAND_LICENSE = BSD-3c
RPI_USERLAND_LICENSE_FILES = LICENCE
RPI_USERLAND_INSTALL_STAGING = YES
RPI_USERLAND_CONF_OPT = -DVMCS_INSTALL_PREFIX=/usr

# we're downloading a branchname rather than a commit-id, so this gets the commit-id we just downloaded
define RPI_USERLAND_GET_CURRENT_VERSION
	echo "rpi-userland Git HEAD @ "`git ls-remote --heads https://github.com/raspberrypi/userland | grep refs/heads/$(RPI_USERLAND_VERSION)` > $(BR2_DL_DIR)/rpi-userland-head.version
endef

RPI_USERLAND_POST_DOWNLOAD_HOOKS += RPI_USERLAND_GET_CURRENT_VERSION

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
