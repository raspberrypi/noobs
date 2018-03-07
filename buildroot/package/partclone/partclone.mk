################################################################################
#
# partclone
#
################################################################################

PARTCLONE_VERSION = 0.2.84
PARTCLONE_SOURCE = partclone-$(PARTCLONE_VERSION).tar.gz
PARTCLONE_SITE = http://downloads.sourceforge.net/project/partclone/source
PARTCLONE_LICENSE = GPLv2
PARTCLONE_LICENSE_FILES = COPYING

define PARTCLONE_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)/src partclone.restore 
endef

define PARTCLONE_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/src/partclone.restore $(TARGET_DIR)/usr/bin/partclone.restore
endef

$(eval $(autotools-package))
