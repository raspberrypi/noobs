# NOOBS (New Out of Box Software)
#### An easy Operating System installer for the Raspberry Pi

NOOBS is designed to make it easy to select and install operating systems for the Raspberry Pi without having to worry about manually imaging your SD card.

The latest official release of NOOBS can be downloaded from http://downloads.raspberrypi.org/NOOBS_latest

For information on previous releases and version changelists, visit https://github.com/raspberrypi/noobs/releases

![alt text](http://downloads.raspberrypi.org/NOOBS/screenshots/os_installed.png "NOOBS Interface")

### About
On first boot NOOBS will format your SD card and allow you to select which OSes you want to install from a list. This OS list is automatically generated from both locally available OSes (i.e. those contained in the `/os` directory on disk) or those available from our remote repository (wired network connection required).

Only the latest version of each OS will ever be displayed meaning that you can be sure that you have installed the most up-to-date release of your selected OS.

On any subsequent boot you can then press the SHIFT key to enter the NOOBS interface and easily reinstall your choice of OSes.

The NOOBS interface provides the following functionality:
- <b>Install</b>: Installs the selected OSes onto your SD card. Changing this selection erases all OSes currently installed.
- <b>Edit Config</b>: Opens a text editor allowing the cmdline and config for the selected installed OS to be edited.
- <b>Online Help</b>: [Networking Required] Open a browser that displays the Raspberry Pi Forum ( http://www.raspberrypi.org/phpBB3/ ), allowing people to quickly access help and troubleshooting.
- <b>Exit</b>: Quits NOOBS and reboots the Pi into the OS boot menu.
- <b>Language Selection</b>: Allows you to select the language to be displayed.
- <b>Keyboard Layout Selection</b>: Allows you to select the keyboard layout to be used.
- <b>Display Mode Selection</b>: By default, NOOBS will output over HDMI at your display's preferred resolution, even if no HDMI display is connected. If you do not see any output on your HDMI display or are using the composite output, press 1, 2, 3 or 4 on your keyboard to select HDMI preferred mode, HDMI safe mode, composite PAL mode or composite NTSC mode respectively.

Note that all user settings (language, keyboard layout, display mode) will persist between reboots and will also be automatically passed to the installed OSes. This means that if you can see the NOOBS interface on your display device then you should be able to see the OS CLI/GUI when it boots too!
### Setup

To setup a blank SD card with NOOBS:
- Format an SD card that is 4GB or greater in size as FAT (see instructions on how to do this below)
- Download and extract the files from the NOOBS zip file.
- Copy the extracted files onto the SD card that you just formatted so that this file is at the root directory of the SD card.
<b> Please note that in some cases it may extract the files into a folder, if this is the case then please copy across the files from inside the folder rather than the folder itself.</b>

On first boot the "RECOVERY" FAT partition will be automatically resized to a minimum and a list of OSes that are available to install will be displayed.

### How to Format an SD card as FAT

For <b>Windows</b> users, we recommend formatting your SD card using the SD Association's Formatting Tool, which can be downloaded from https://www.sdcard.org/downloads/formatter_4/ You will need to set "FORMAT SIZE ADJUSTMENT" option to "ON" in the "Options" menu to ensure that the entire SD card volume is formatted - not just a single partition. For more detailed and beginner-friendly formatting instructions, please refer to http://www.raspberrypi.org/quick-start-guide

The SD Association's Formatting Tool is also available for <b>Mac</b> users although the default OSX Disk Utility is also capable of formatting the entire disk (select the SD card volume and choose "Erase" with "MS-DOS" format).

For <b>Linux</b> users we recommend `gparted` (or the command line version `parted`). (Update: Norman Dunbar has written up the following formatting instructions for Linux users: http://qdosmsq.dunbar-it.co.uk/blog/2013/06/noobs-for-raspberry-pi/ )

===

### Screenshots

#### OS Installation

Simply select the checkbox next to each OS you want to install using either a mouse or keyboard (arrow keys to traverse the list, enter to toggle the selected OS's checkbox), then click the "Install" icon (or press "i" on your keyboard) to install the selection. The icons shown on the right of the list indicate whether the OS is being installed from the SD card (SD card icon) or from the online OS repository (Ethernet icon).

![alt text](http://downloads.raspberrypi.org/NOOBS/screenshots/os_selected.png "Select your choice of OSes to install")

#### Online Help via Web Browser

The built-in Arora web browser allows you to easily get help via the Raspberry Pi Forums (wired network connection required).

![alt text](http://downloads.raspberrypi.org/NOOBS/screenshots/browser.png "Search the Raspberry Pi forums for help via the built-in web browser")

#### Easy Config File Editor

The built-in config file editor allows you to edit the config file of the OS currently highlighted in the OS list. This allows you to easily add license keys to different OS installs through the same interface.

Note that the output mode selected by the user through pressing one of number keys 1 to 4 (for HDMI preferred, HDMI VGA, Composite PAL and Composite NTSC respectively), will be automatically set in the `config.txt` files of your installed OSes. This means that you shouldn't have to worry about manually changing your display settings to get your installed OS to display correctly on your display device.

![alt text](http://downloads.raspberrypi.org/NOOBS/screenshots/config_editor.png "Easily edit the config files of any installed OS")

#### Installer Slideshow

An installer slideshow guides you through your first steps with each OS while it installs.

![alt text](http://downloads.raspberrypi.org/NOOBS/screenshots/installer_slides.png "An installer slideshow guides you through your first steps with each OS")

#### OS Boot Selector

After multiple OSes have been installed, you can select which OS to boot through this selection window that is automatically displayed. NOOBS will remember your choice and boot this OS by default unless a different option has been selected within 10 seconds.

Note that if only one OS is installed then the boot selector will not be displayed and the OS will be automatically booted.

![alt text](http://downloads.raspberrypi.org/NOOBS/screenshots/boot_select.png "Easily select which OS you want to boot from a list of those currently installed")

==

## Advanced Usage (for experts and teachers)

### How to Automatically Install an OS

Even if you are using your Pi without a display, you can still use NOOBS to easily install an OS of your choice. To set up NOOBS to automatically and silently (i.e. without requiring any user input) install a specific OS, follow these steps:

1. Copy the OS folder for the OS you want to install into the `/os` dir (or alternatively delete all other OSes contained in the `/os` dir so that only your chosen OS remains.

2. If the OS you want to automatically install has multiple flavours available, edit the `flavours.json` file so that it only contains the flavour entry that you want to install.

3. Edit the `recovery.cmdline` file in the root NOOBS directory and append `silentinstall` to the arguments list.

When you now boot your Pi using an SD card containing the modified version of NOOBS that you just created, it will automatically install the OS you chose and boot into it after the installation has finished.
(Note that there's currently a small bug in NOOBS which means _if_ you use silentinstall with the Ethernet connected, then after NOOBS finishes installing the OS, it crashes rather than rebooting as expected. Simply power off and on again, and your chosen OS will boot as expected.)

### How to create a custom OS version

There are two main use cases for which you may want to create a custom version of one of the standard OS releases that is suitable for installation via NOOBS:
- If you are a teacher wanting to easily deploy a custom OS release containing pre-defined set of packages and files onto a number of SD cards (e.g. to provision a class set of Raspberry Pi's or quickly restore a Raspberry Pi back to custom "factory" settings).
- If you want to be able to back up your existing installed packages and files so that any future OS re-install does not force you back to a clean install.

The following steps allow you to create a modified copy of one of the standard OS releases that contains your custom files, packages and settings.

1. Download a base version of NOOBS from http://downloads.raspberrypi.org/NOOBS_latest

2. Extract the NOOBS zipfile

3. Navigate to the `os` directory

4. Create a copy of the folder containing the OS release that you want to modify and rename it with a custom name.

5. Edit the following fields in the `os.json` file contained in the folder that you just created
  1. "name" - replace the name of the base OS with the name of your custom OS version
  2. "description" - replace the description of the standard OS install with one for your custom OS version

6. [Optional] Rename or replace the existing `<OS>.png` icon file with one matching the name of your custom OS version

7. [Optional] Replace the PNG image files in the `slides` and `slides_vga` directory with your own custom installer slides

8. Edit the following fields in the `partitions.json` file contained in the folder that you just created
  1. "partition_size_nominal" - replace the numerical value with the size of the paritions in your custom OS version
  2. "uncompressed_tarball_size" - replace the numerical value with the size of your filesystem tarballs when uncompressed

9. Replace the `.tar.xz` root and boot filesystem tarballs with copies created from your custom OS version (these instructions assume you're only using a single OS at a time with NOOBS - they won't work if you're running multiple OSes from a single SD card). The name of these tarballs needs to match the labels given in `partitions.json`.
  1. To create the root tarball you will need to run `tar -cvpf <label>.tar /* --exclude=proc/* --exclude=sys/* --exclude=dev/pts/*` from within the root filesystem of your custom OS version. You should then compress the resulting tarball with `xz -9 -e <label>.tar`.
  2. To create the boot tarball you will need to run `tar -cvpf <label>.tar .` at the root directory of the boot partition of your custom OS version. You should then compress the resulting tarball with `xz -9 -e <label>.tar`.

### How to change the default Language, Keyboard layout, Display mode or Boot Partition

Edit the `recovery.cmdline` file in the root NOOBS directory and append the following arguments where relevant:
- `lang=<two-letter language code>` (e.g. `lang=de` or `lang=en`)
- `keyboard=<two-letter layout code>` (e.g. `keyboard=de` or `keyboard=us`)
- `display=<display mode number>` (e.g. `display=1` or `display=3`)
- `partition=<partition_number>` (e.g. `partition=5`)

Note that these defaults will be overwritten by any changes made in the GUI to these settings.

### How to bypass the Recovery splashscreen and boot directly into a fixed partition

After you have installed your chosen OSes, add the following file to the root directory of NOOBS to force the indicated partition to be booted at power-on.

1. Add a text file named `autoboot.txt` to the root directory of NOOBS.

2. Add `boot_partition=<partition number>` to the file and save it to disk.

This will also prevent the splashscreen from being displayed at boot. The partition number can be found by running `sudo fdisk -l` the partition will be one of the FAT32 partitions `/dev/mmcblk0p5` would be partition 5.

===

## Troubleshooting

#### What to do if your SHIFT keypress isn't detected

Try pressing shift only when the grey splashscreen is displayed rather than holding it from boot up.

#### How to boot into "Safe Mode"

To boot into a basic busybox shell rather than launching the NOOBS GUI, you can either:

1. Append `rescueshell` to the argument list in the `recovery.cmdline` file which is found in the root NOOBS directory.

2. Insert a physical jumper between pins 5 & 6 of GPIO header P1.

#### How to enable using the GPIO to trigger entering Recovery Mode

To force Recovery Mode to be entered on boot and to show the NOOBS interface, you normally press the `SHIFT` key during bootup. If you don't have a keyboard or the `SHIFT` keypress isn't being detected, you should complete the following steps to force the NOOBS interface to be displayed on boot:

1. Append `gpiotriggerenable` to the argument list in the `recovery.cmdline` file which is found in the root NOOBS directory.
2. Reboot

To force Recovery Mode being entered on boot, connect GPIO pin 3 on header P1 to GND (pin 25). If GPIO pin 3 remains unconnected then it will boot through to the installed OS as normal.

#### How to force Recovery Mode being entered on boot (overrides GPIO or keyboard input)

Alternatively, if you are unable to use either the GPIO or keyboard to trigger entering Recovery Mode, you can:

1. Append `forcetrigger` to the argument list in the `recovery.cmdline` file which is found in the root NOOBS directory.
2. Reboot

Note that with this option enabled, the Recovery Mode will be displayed <b>every</b> time you boot from your NOOBS card (until you edit `recovery.cmdline` again).

#### How to disable using the keyboard to trigger entering Recovery Mode

In some rare cases, you may find that NOOBS incorrectly detects a `SHIFT` keypress from your keyboard regardless of the presence of user input. In such cases it may be helpful to disable using the keyboard to trigger Recovery Mode being entered.

To prevent a `SHIFT` keypress from entering Recovery Mode on boot (maybe you have a problematic keyboard which is erroneously triggering every time you boot), you can:

1. Append `keyboardtriggerdisable` to the argument list in the `recovery.cmdline` file which is found in the root NOOBS directory.
2. Reboot

#### How to change display output modes

By default, NOOBS will output over HDMI at your display’s preferred resolution, even if no HDMI display is connected. If you do not see any output on your HDMI display or are using the composite output, press 1, 2, 3 or 4 on your keyboard to select HDMI preferred mode, HDMI safe mode, composite PAL mode or composite NTSC mode respectively.

If you don't have a keyboard, you can still change the display mode used by NOOBS through editing the `recovery.cmdline` file in the root NOOBS directory prior to first boot and appending the following argument:
- `display=<display mode number>` (e.g. `display=1` or `display=3`)

===

## How to Rebuild NOOBS

Note that this will require a minimum of 6GB free disk space.

#### Get Build Dependencies

On Ubuntu:

`sudo apt-get install build-essential rsync texinfo libncurses-dev whois unzip`

#### Run Build Script

`./BUILDME.sh`

Buildroot will then build the software and all dependencies, putting the result in the `output` directory.

Buildroot by default compiles multiple files in parallel, depending on the number of CPU cores you have.

If your build machine does have a quad core CPU, but relatively little RAM, you may want
to lower the number to prevent swapping:
- `cd buildroot ; make menuconfig`
- "Build options" -> "Number of jobs to run simultaneously"

## How to run your Build

In order to setup an SD card with a newly built version of NOOBS, you will need to:
- Format an SD card that is 4GB or greater in size as FAT
- Replace the `/os` directory in `/output` with the copy contained in the release version of NOOBS (see above for download links)
- Copy the files in the `/output` directory onto the SD card

## About the Buildroot infrastructure

To add extra packages: `cd buildroot ; make menuconfig`

Recovery software packaging is in: `buildroot/package/recovery`

Kernel configuration used: `buildroot/kernelconfig-recovery`

Main differences with bcmrpi_defconfig:
- `CONFIG_BLK_DEV_INITRD=y` - initramfs support
- `CONFIG_INPUT_EVDEV=y` - evdev support built-in
- `CONFIG_USB_HID=y` - usb HID driver built-in
- All modules disabled.

## Modifying Qt source

Source is in the `recovery` folder.
Be aware that user interface screens will appear larger in Qt Creator then when deployed on the Pi, can
raise font sizes 2 points to compensate.

Several constants can be changed in `config.h`

Wrap code that calls Qt Embedded specific classes (such as QWSServer) between
```C
#ifdef Q_WS_QWS
```
and
```C
#endif
```
so that the project also compiles and can be tested under standard Qt.

## Adding/Updating Translations

References:

http://qt-project.org/doc/qt-4.8/i18n-source-translation.html

http://qt-project.org/doc/qt-4.8/linguist-manual.html

To set up a git pre-commit hook to automatically update the translation files, run the following commands in the project root:
- `chmod +x pre-commit-translation-update-hook.sh`
- `cp pre-commit-translation-update-hook.sh .git/hooks/pre-commit`

To add a new translation:
- Add to `recovery/recovery.pro` the following: `TRANSLATIONS += translation_<languagecode>.ts`
- Run `lupdate recovery/recovery.pro` which extracts strings from the source code and generates/updates the *.ts* files.
- The *.ts* can then be sent to the translator, opened in Qt Linguist and filled in.
- Add a line for the *.ts* file in to `recovery/icons.qrc`, but substitute *.ts* extension with *.qm* . This file contains a list
  of resource files that will be embedded into the application's executable during build.
- Add a flag icon for your language from http://www.famfamfam.com/lab/icons/flags/ flag icon collection or if it
  doesn't have the one you need, you may use some other small png icon for it. Copy the icon file to the `recovery/icons`
  folder and add a line for it into `recovery/icons.qrc` as well.


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
- http://www.oxygen-icons.org/ - Available under Creative Common Attribution-ShareAlike 3.0 and LGPL license

Licenses of utility software build by buildroot:
Type `cd buildroot ; make legal-info` to generate a list, which will be available under `output/legal-info`.
