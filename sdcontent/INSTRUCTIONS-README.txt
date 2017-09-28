NOOBS INSTALLATION INSTRUCTIONS

1. Insert an SD card that is 8GB or greater in size into your computer.
2. Format the SD card using the platform-specific instructions below:
   a. Windows (32GB cards and under)
      i. Download the SD Association's Formatting Tool from https://www.sdcard.org/downloads/formatter_4/eula_windows/
      ii. Install and run the Formatting Tool on your machine
      iii. Check that the SD card you inserted matches the one selected by the Tool
      iv. Click the "Format" button
   b. Mac (32GB cards and under)
      i. Download the SD Association's Formatting Tool from https://www.sdcard.org/downloads/formatter_4/eula_mac/
      ii. Install and run the Formatting Tool on your machine
      iii. Select "Overwrite Format"
      iv. Check that the SD card you inserted matches the one selected by the Tool
      v. Click the "Format" button
   c. Linux
      i. We recommend using gparted (or the command line version parted)
      ii. Format the entire disk as FAT32
   d. Cards over 32GB
      i. Follow the instructions on https://www.raspberrypi.org/documentation/installation/sdxc_formatting.md
3. Extract the files contained in this NOOBS zip file.
4. Copy the extracted files onto the SD card that you just formatted so that this file is at the root directory of the SD card. Please note that in some cases it may extract the files into a folder, if this is the case then please copy across the files from inside the folder rather than the folder itself.
5. Insert the SD card into your Pi and connect the power supply.

Your Pi will now boot into NOOBS and should display a list of operating systems that you can choose to install.
If your display remains blank, you should select the correct output mode for your display by pressing one of the following number keys on your keyboard:
1. HDMI mode - this is the default display mode.
2. HDMI safe mode - select this mode if you are using the HDMI connector and cannot see anything on screen when the Pi has booted.
3. Composite PAL mode - select either this mode or composite NTSC mode if you are using the composite RCA video connector.
4. Composite NTSC mode

If you are still having difficulties after following these instructions, then please visit the Raspberry Pi Forums ( http://www.raspberrypi.org/forums/ ) for support.
