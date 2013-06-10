#############################################################
#
# Arora 
#
#############################################################

ARORA_SITE=git://github.com/Arora/arora.git
ARORA_VERSION=master
ARORA_LICENSE = GPLv2
ARORA_LICENSE_FILES = LICENSE.GPL2
ARORA_INSTALL_STAGING=NO
ARORA_DEPENDENCIES=qt

define ARORA_BUILD_CMDS
	(cd $(@D) ; $(QT_QMAKE))
	$(MAKE) -C $(@D) all
	$(TARGET_STRIP) $(@D)/arora
endef

define ARORA_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(@D)/arora $(TARGET_DIR)/usr/bin/arora
endef

$(eval $(generic-package))
