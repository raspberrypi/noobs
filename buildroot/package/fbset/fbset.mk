################################################################################
#
# fbset
#
################################################################################

FBSET_VERSION = 2.1
FBSET_SITE = http://users.telenet.be/geertu/Linux/fbdev
FBSET_DEPENDENCIES = host-bison host-flex
FBSET_LICENSE = GPLv2

# Make sure full fbset wins over busybox fbset
ifeq ($(BR2_PACKAGE_BUSYBOX),y)
FBSET_DEPENDENCIES += busybox
endif

define FBSET_BUILD_CMDS
	$(MAKE1) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

define FBSET_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 755 $(@D)/fbset $(TARGET_DIR)/usr/sbin/fbset
	$(INSTALL) -D -m 755 $(@D)/con2fbmap $(TARGET_DIR)/bin/con2fbmap
endef

$(eval $(generic-package))
