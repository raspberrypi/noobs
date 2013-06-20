#ifndef CONFIG_H
#define CONFIG_H

/* Version number displayed in the title bar */
#define VERSION_NUMBER "1.2"

/* Color of the background */
// #define BACKGROUND_COLOR  Qt::white
#define BACKGROUND_COLOR  QColor(0xde, 0xde, 0xde)

/* Places background.png resource file in center of screen */
#define CENTER_BACKGROUND_IMAGE

/* Enable language selection */
#define ENABLE_LANGUAGE_CHOOSER

/* Website launched when launching Arora */
#define HOMEPAGE  "http://www.raspberrypi.org/phpBB3/"

/* Location to download the list of available distributions from
 * Can also be a local file on the SD card (which is mounted under /mnt) */
// #define DEFAULT_REPO_SERVER   "http://www.somewebsite.com/distro.ini"
#define DEFAULT_REPO_SERVER  "file:///mnt/distro.ini"

/* Size of recovery FAT partition in MB.
 * First partition starts at offset 1 MB (sector 2048)
 * If you want the second partition to start at offset 1024 MB, enter 1023 */
#define RESCUE_PARTITION_SIZE  1531

/* Minimum SD card size in MB
 * Throws an error if user tries to use a smaller one  */
#define MINIMUM_SDCARD_SIZE  3072

/* Files that are currently on the FAT partition are normaaly saved to memory during
 * repartitioning.
 * If files they are larger than number of MB, try resizing the FAT partition instead */
#define MAXIMUM_BOOTFILES_SIZE  64

/* Print out messages showing wheter or not keyboard and key press were detected */
// #define VERBOSE_KEYPRESS_DETECTION

/* If the image has this word in the name, mark it as recommended */
#define RECOMMENDED_IMAGE "Raspbian"

/* Do we want extended partitions or overwrite the main MBR? */
#define USE_EXTENDED_PARTITIONS

#ifdef USE_EXTENDED_PARTITIONS
#define FAT_PARTITION_OF_IMAGE  "/dev/mmcblk0p5"
#else
#define FAT_PARTITION_OF_IMAGE  "/dev/mmcblk0p2"
#endif


/* RiscOS magic */
#define RISCOS_SECTOR_OFFSET (1536 * 2048)
#define RISCOS_FAT_SIZE      (48 * 2048)

#define RISCOS_BLOB_FILENAME  "/mnt/riscos-boot.bin"
#define RISCOS_BLOB_SECTOR_OFFSET  (1)

#endif // CONFIG_H
