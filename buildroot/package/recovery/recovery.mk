#############################################################
#
# Recovery system
#
#############################################################


RECOVERY_VERSION = 1.0
RECOVERY_SITE = $(TOPDIR)/../recovery
RECOVERY_SITE_METHOD = local
RECOVERY_LICENSE = BSD-3c
RECOVERY_LICENSE_FILES = LICENSE.txt
RECOVERY_INSTALL_STAGING = NO
RECOVERY_DEPENDENCIES = qt qjson

define RECOVERY_BUILD_CMDS
	(cd $(@D) ; $(QT_QMAKE))
	$(MAKE) -C $(@D) all
	$(TARGET_STRIP) $(@D)/recovery
endef

define RECOVERY_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(@D)/recovery $(TARGET_DIR)/usr/bin/recovery
	$(INSTALL) -D -m 0644 package/recovery/interfaces $(TARGET_DIR)/etc/network/interfaces
	rm $(TARGET_DIR)/init || true
	$(INSTALL) -m 0755 package/recovery/init $(TARGET_DIR)/init
	rm -f $(TARGET_DIR)/usr/lib/fonts/*
	mkdir -p $(TARGET_DIR)/usr/lib/fonts/
	$(INSTALL) -m 0755 package/recovery/unicode-fonts/DejaVuSans.ttf $(TARGET_DIR)/usr/lib/fonts/DejaVuSans.ttf
	$(INSTALL) -m 0755 package/recovery/unicode-fonts/DejaVuSans-Bold.ttf $(TARGET_DIR)/usr/lib/fonts/DejaVuSans-Bold.ttf
	$(INSTALL) -m 0755 package/recovery/unicode-fonts/DroidSansJapanese.ttf $(TARGET_DIR)/usr/lib/fonts/DroidSansJapanese.ttf
	$(INSTALL) -m 0755 package/recovery/data/data $(TARGET_DIR)/usr/data
	$(INSTALL) -m 0644 $(@D)/cmdline.txt $(BINARIES_DIR)/cmdline.txt
	mkdir -p $(TARGET_DIR)/keymaps/
	$(INSTALL) -m 0755 package/recovery/keymaps/* $(TARGET_DIR)/keymaps/
endef

$(eval $(generic-package))
