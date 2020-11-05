#############################################################
#
# con2fbmap system
#
#############################################################


CON2FBMAP_VERSION = 1.0
CON2FBMAP_SITE = $(TOPDIR)/../con2fbmap
CON2FBMAP_SITE_METHOD = local
CON2FBMAP_LICENSE = BSD-3c
CON2FBMAP_LICENSE_FILES = LICENSE.txt
CON2FBMAP_INSTALL_STAGING = NO

define CON2FBMAP_BUILD_CMDS
	cd $(@D) 
	$(MAKE) -C $(@D) all
	$(TARGET_STRIP) $(@D)/con2fbmap
endef

define CON2FBMAP_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(@D)/con2fbmap $(TARGET_DIR)/usr/bin/con2fbmap
endef

$(eval $(generic-package))
