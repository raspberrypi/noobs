################################################################################
#
# rpi-userland
#
################################################################################

RPI_USERLAND_VERSION = 3b81b91c18ff19f97033e146a9f3262ca631f0e9
RPI_USERLAND_SITE = $(call github,raspberrypi,userland,$(RPI_USERLAND_VERSION))
RPI_USERLAND_LICENSE = BSD-3c
RPI_USERLAND_LICENSE_FILES = LICENCE
RPI_USERLAND_INSTALL_STAGING = YES
RPI_USERLAND_CONF_OPTS = -DVMCS_INSTALL_PREFIX=/usr \
	-DCMAKE_C_FLAGS="-DVCFILED_LOCKFILE=\\\"/var/run/vcfiled.pid\\\""

RPI_USERLAND_PROVIDES = libegl libgles libopenmax libopenvg

ifeq ($(BR2_PACKAGE_RPI_USERLAND_START_VCFILED),y)
define RPI_USERLAND_INSTALL_INIT_SYSV
	$(INSTALL) -m 0755 -D package/rpi-userland/S94vcfiled \
		$(TARGET_DIR)/etc/init.d/S94vcfiled
endef
endif

define RPI_USERLAND_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(STAGING_DIR)/usr/lib/libvchiq_arm.so $(TARGET_DIR)/usr/lib
	$(INSTALL) -m 0755 $(STAGING_DIR)/usr/lib/libvcos.so $(TARGET_DIR)/usr/lib
	$(INSTALL) -m 0755 $(STAGING_DIR)/usr/lib/libvchostif.so $(TARGET_DIR)/usr/lib
	$(INSTALL) -m 0755 $(STAGING_DIR)/usr/lib/libvcfiled_check.so $(TARGET_DIR)/usr/lib
	$(INSTALL) -m 0755 $(STAGING_DIR)/usr/bin/tvservice $(TARGET_DIR)/usr/bin
	$(INSTALL) -m 0755 $(STAGING_DIR)/usr/bin/vcgencmd $(TARGET_DIR)/usr/bin
endef

$(eval $(cmake-package))
