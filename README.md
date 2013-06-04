# Raspberry Pi Recovery System
#### (New Out Of Box Software)

![alt text](http://www.raspberrypi.org/wp-content/uploads/2013/06/mainwindow.png "NOOBS Screenshot")

The Raspberry Pi Recovery System is designed to make it easy to select and install operating systems for the Raspberry Pi on an SD card without requiring either network access or a more complicated SD card imaging operation.

### About
The Recovery system works by creating a FAT partition on your SD card containing heavily compressed versions of OS images. At boot you can press SHIFT to enter the recovery app and easily reinstall the existing OS or choose to install a different one.

The Recovery System currently has the following functionality;
- <b>Restore Image</b>: Writes the selected OS image onto the remainder of the SD card. Only one OS can be imaged at a time.
- <b>Edit Config</b>: Opens a text editor allowing the cmdline and config for the installed OS to be edited.
- <b>Online Help</b>: [Networking Required] Open a browser that displays the raspberrypi.org/forum, allowing people to quickly access help and troubleshooting.
- <b>Exit</b>: Quits the recovery app and reboots the Pi into the installed OS.

### Setup

To setup a blank SD card with the Recovery System;
- Format an SD card that is 4GB or greater in size as FAT32
- Copy the files in the `/output` directory onto the SD card
- Place any OS images you want to be able to install into the `/images` directory on the SD card

On first boot the "Recovery" FAT partition will be automatically resized to a minimum and a list of OS images that are available to install will be displayed. If there is only one OS image contained within `/images` then this will be automatically installed on first boot.

### How to Format an SD card as FAT

For <b>Windows</b> users, we recommend formatting your SD card using the SD Association's Formatting Tool, which can be downloaded from https://www.sdcard.org/downloads/formatter_4/ You will need to set "FORMAT SIZE ADJUSTMENT" option to "ON" in the "Options" menu to ensure that the entire SD card volume is formatted - not just a single partition. For more detailed and beginner-friendly formatting instructions, please refer to http://www.raspberrypi.org/quick-start-guide.

The SD Association's Formatting Tool is also available for <b>Mac</b> users although the default OSX Disk Utility is also capable of formatting the entire disk (select the SD card volume and choose "Erase" with "MS-DOS" format).

For <b>Linux</b> users we recommend `gparted` (or the command line version `parted`). (Update: Norman Dunbar has written up the following formatting instructions for Linux users: http://qdosmsq.dunbar-it.co.uk/blog/2013/06/noobs-for-raspberry-pi/) 

===

## How to Rebuild the Recovery System

#### Get Build Dependencies

On Ubuntu:

`sudo apt-get install build-essential rsync texinfo libncurses-dev whois`

#### Run Build Script

`./BUILDME.sh`

Buildroot will then build the software and all dependencies, putting the result in the "output" directory.

Buildroot by default compiles multiple files in parallel, depending on the number of CPU cores you have.

If your build machine does have a quad core CPU, but relatively little RAM, you may want
to lower the number to prevent swapping.

`cd buildroot ; make menuconfig`

"Build options" -> "Number of jobs to run simultaneously"

## About the Buildroot infrastructure

To add extra packages: `cd buildroot ; make menuconfig`

Recovery software packaging is in: `buildroot/package/recovery`

Kernel configuration used: `buildroot/kernelconfig-recovery`

Main differences with bcmrpi_defconfig:

- `CONFIG_BLK_DEV_INITRD=y` - initramfs support
- `CONFIG_INPUT_EVDEV=y` - evdev support built-in
- `CONFIG_USB_HID=m` - usb HID driver and alternative driver build as module
- `CONFIG_USB_KBD=m`
- All other modules disabled.

## Modifying Qt source

Source is in the "recovery" folder.
Be aware that user interface screens will appear larger in Qt Creator then when deployed on the Pi, can
raise font sizes 2 points to compensate.

Several constants can be changed in "config.h"

Wrap code that calls Qt Embedded specific classes (such as QWSServer) between "#ifdef Q_WS_QWS" and "#endif"
So that the project also compiles and can be tested under standard Qt.

## Adding/Updating Translations

References:

http://qt-project.org/doc/qt-4.8/i18n-source-translation.html

http://qt-project.org/doc/qt-4.8/linguist-manual.html

To add a new translation:

- Add to recovery/recovery.pro `TRANSLATIONS += translation_<languagecode>.ts`
- Run `lupdate recovery.pro` which extracts strings from the source code and generates/updates the .ts files.
- The .ts can then be send to the translator, opened in Qt Linguist and filled in.
- Turn the .ts XML file into a binary .qm file by running `lrelease translation_code.ts`
- The .qm file needs to be added to "icons.qrc". This file contains a list with resource files that will be embedded into the
  application's executable during build.

Can add a flag icon to the "icons" folder and add that flag to "icons.qrc" as well.


### Legal compliance

Copyright (c) 2013, Raspberry Pi
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the Raspberry Pi Foundation nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#### Third party licenses:

Recovery software directly links to:

- Qt libraries, available under LGPL and commercial license.

Currently used icon sets:

- http://www.fatcow.com/free-icons - Creative commons Attribution license
- http://www.famfamfam.com/lab/icons/flags - "These flag icons are available for free use for any purpose with no requirement for attribution."

Licenses of utility software build by buildroot:

Type `cd buildroot ; make legal-info` to generate a list, which will be available under `output/legal-info`.
