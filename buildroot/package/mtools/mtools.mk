#############################################################
#
# mtools
#
#############################################################

MTOOLS_VERSION       = 4.0.18
MTOOLS_SOURCE        = mtools-$(MTOOLS_VERSION).tar.bz2
MTOOLS_SITE          = $(BR2_GNU_MIRROR)/mtools/
MTOOLS_LICENSE       = GPLv3+
MTOOLS_LICENSE_FILES = COPYING
MTOOLS_LDFLAGS       = $(TARGET_LDFLAGS)

MTOOLS_CONF_OPT = --enable-xdf        \
                  --enable-vold

ifneq ($(BR2_ENABLE_LOCALE),y)
MTOOLS_DEPENDENCIES += libiconv
MTOOLS_LDFLAGS += -liconv
endif

define MTOOLS_BUILD_CMDS
  $(MAKE) CC="$(TARGET_CC)" LDFLAGS="$(MTOOLS_LDFLAGS)" -C $(@D)
endef

define MTOOLS_INSTALL_TARGET_CMDS
  $(INSTALL) -m 0755 $(@D)/mlabel $(TARGET_DIR)/sbin/mlabel
	$(INSTALL) -m 0755 package/mtools/.mtoolsrc $(TARGET_DIR)/.mtoolsrc
	$(INSTALL) -m 0755 package/mtools/mtools.conf $(TARGET_DIR)/etc/mtools.conf
endef

$(eval $(autotools-package))
