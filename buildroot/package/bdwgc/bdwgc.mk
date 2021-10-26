################################################################################
#
# bdwgc
#
################################################################################

# When bumping the version number, check if the list of supported architectures
# is still valid; see Config.log.
BDWGC_VERSION = 8.0.6
BDWGC_SOURCE = gc-$(BDWGC_VERSION).tar.gz
BDWGC_SITE = https://www.hboehm.info/gc/gc_source
BDWGC_INSTALL_STAGING = YES
BDWGC_LICENSE = bdwgc license
BDWGC_LICENSE_FILES = README.QUICK
BDWGC_DEPENDENCIES = libatomic_ops host-pkgconf

# The libtool shipped with the package is bogus and generates some
# -L/usr/lib flags. It uses a version not supported by Buildroot
# libtool patches, so autoreconfiguring the packages is the easiest
# solution.
BDWGC_AUTORECONF = YES

$(eval $(autotools-package))
$(eval $(host-autotools-package))
