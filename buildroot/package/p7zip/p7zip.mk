################################################################################
#
# p7zip
#
################################################################################

P7ZIP_VERSION = 9.38.1
P7ZIP_SOURCE = p7zip_$(P7ZIP_VERSION)_src_all.tar.bz2
P7ZIP_SITE = http://downloads.sourceforge.net/project/p7zip/p7zip/$(P7ZIP_VERSION)
P7ZIP_LICENSE = LGPLv2.1+
P7ZIP_LICENSE_FILES = docs/License.txt

define P7ZIP_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" CXX="$(TARGET_CXX)" \
        -C $(@D) 7z
endef

define P7ZIP_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 755 $(@D)/bin/7z $(TARGET_DIR)/usr/lib/p7zip/7z
    $(INSTALL) -D -m 755 $(@D)/bin/7z.so $(TARGET_DIR)/usr/lib/p7zip/7z.so
    $(INSTALL) -D -m 755 package/p7zip/7z $(TARGET_DIR)/usr/bin/7z
endef

$(eval $(generic-package))
