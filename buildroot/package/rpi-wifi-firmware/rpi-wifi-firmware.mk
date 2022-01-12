################################################################################
#
# rpi-wifi-firmware
#
################################################################################

RPI_WIFI_FIRMWARE_VERSION = 20210315-3+rpt4
#RPI_WIFI_FIRMWARE_SITE = $(call github,RPi-Distro,firmware-nonfree,$(RPI_WIFI_FIRMWARE_VERSION))
RPI_WIFI_FIRMWARE_SITE = http://archive.raspberrypi.org/debian/pool/main/f/firmware-nonfree/
RPI_WIFI_FIRMWARE_SOURCE = firmware-brcm80211_$(RPI_WIFI_FIRMWARE_VERSION)_all.deb
RPI_WIFI_FIRMWARE_LICENSE = Proprietary
RPI_WIFI_FIRMWARE_LICENSE_FILES = LICENCE.broadcom_bcm43xx

define RPI_WIFI_FIRMWARE_EXTRACT_CMDS
	dpkg-deb -x $(DL_DIR)/$(RPI_WIFI_FIRMWARE_SOURCE) $(@D)
endef

define RPI_WIFI_FIRMWARE_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43143.bin $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43143.bin
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43430-sdio.bin $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43430-sdio.bin
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43430-sdio.txt $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43430-sdio.txt
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43430-sdio.clm_blob $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43430-sdio.clm_blob
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43436-sdio.bin $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43436-sdio.bin
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43436-sdio.txt $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43436-sdio.txt
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43436-sdio.clm_blob $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43436-sdio.clm_blob
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43455-sdio.bin $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43455-sdio.bin
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43455-sdio.txt $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43455-sdio.txt
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43455-sdio.clm_blob $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43455-sdio.clm_blob
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43456-sdio.bin $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43456-sdio.bin
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43456-sdio.txt $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43456-sdio.txt
	$(INSTALL) -D -m 0644 $(@D)/lib/firmware/brcm/brcmfmac43456-sdio.clm_blob $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43456-sdio.clm_blob
endef

$(eval $(generic-package))
