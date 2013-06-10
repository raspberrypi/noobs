#############################################################
#
# dosfstools
#
#############################################################

DOSFSTOOLS_VERSION = 3.0.16
DOSFSTOOLS_SITE = http://daniel-baumann.ch/files/software/dosfstools
DOSFSTOOLS_LICENSE = GPLv3+
DOSFSTOOLS_LICENSE_FILES = COPYING
DOSFSTOOLS_LDFLAGS = $(TARGET_LDFLAGS)

# Avoid target dosfstools dependencies, no host-libiconv
HOST_DOSFSTOOLS_DEPENDENCIES =

ifneq ($(BR2_ENABLE_LOCALE),y)
DOSFSTOOLS_DEPENDENCIES += libiconv
DOSFSTOOLS_LDFLAGS += -liconv
endif

MKDOSFS_BINARY = mkdosfs
DOSFSCK_BINARY = dosfsck
DOSFSLABEL_BINARY = dosfslabel

define DOSFSTOOLS_BUILD_CMDS
	$(MAKE) CFLAGS="$(TARGET_CFLAGS)" CC="$(TARGET_CC)" \
		LDFLAGS="$(DOSFSTOOLS_LDFLAGS)" -C $(@D)
endef

DOSFSTOOLS_INSTALL_BIN_FILES_$(BR2_PACKAGE_DOSFSTOOLS_MKDOSFS)+=$(MKDOSFS_BINARY)
DOSFSTOOLS_INSTALL_BIN_FILES_$(BR2_PACKAGE_DOSFSTOOLS_DOSFSCK)+=$(DOSFSCK_BINARY)
DOSFSTOOLS_INSTALL_BIN_FILES_$(BR2_PACKAGE_DOSFSTOOLS_DOSFSLABEL)+=$(DOSFSLABEL_BINARY)

define DOSFSTOOLS_INSTALL_TARGET_CMDS
	test -z "$(DOSFSTOOLS_INSTALL_BIN_FILES_y)" || \
	install -m 755 $(addprefix $(@D)/,$(DOSFSTOOLS_INSTALL_BIN_FILES_y)) $(TARGET_DIR)/sbin/
endef

define DOSFSTOOLS_UNINSTALL_TARGET_CMDS
	rm -f $(TARGET_DIR)/sbin/$(MKDOSFS_BINARY)
	rm -f $(TARGET_DIR)/sbin/$(DOSFSCK_BINARY)
	rm -f $(TARGET_DIR)/sbin/$(DOSFSLABEL_BINARY)
endef

define DOSFSTOOLS_CLEAN_CMDS
	-$(MAKE) -C $(@D) clean
endef

define HOST_DOSFSTOOLS_BUILD_CMDS
	$(MAKE) $(HOST_CONFIGURE_OPTS) -C $(@D)
endef

define HOST_DOSFSTOOLS_INSTALL_CMDS
	$(MAKE) -C $(@D) $(HOST_CONFIGURE_OPTS) PREFIX=$(HOST_DIR)/usr install
endef

$(eval $(generic-package))
$(eval $(host-generic-package))
