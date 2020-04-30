NOOBS INSTALLATION INSTRUCTIONS

1. Insert an SD card that is 16GB or greater in size into your computer.
2. Format the SD card using the platform-specific instructions below:
   a. Windows (32GB cards and under)
      i. Download the SD Association's Formatting Tool from https://www.sdcard.org/downloads/formatter_4/eula_windows/
      ii. Install and run the Formatting Tool on your machine
      iii. Check that the SD card you inserted matches the one selected by the Formatting Tool
      iv. Click the "Format" button
   b. Mac (32GB cards and under)
      i. Download the SD Association's Formatting Tool from https://www.sdcard.org/downloads/formatter_4/eula_mac/
      ii. Install and run the Formatting Tool on your machine
      iii. Select "Overwrite Format"
      iv. Check that the SD card you inserted matches the one selected by the Formatting Tool
      v. Click the "Format" button
   c. Linux
      i. We recommend using gparted (or the command line version, parted)
      ii. Format the entire card as FAT32
   d. Cards over 32GB
      i. Follow the instructions at https://www.raspberrypi.org/documentation/installation/sdxc_formatting.md
3. Extract the files contained in this NOOBS zip file.
4. Copy the extracted files onto the SD card that you just formatted. The files must be in the root directory of the SD card, not in a subdirectory within this.
5. Insert the SD card into the Raspberry Pi and connect the power supply.

**Raspberry Pi 4B**:
- If you are using an HDMI display,  you must connect it to the micro HDMI port closest to the power jack, labelled HDMI 0.
- If you are instead connecting a composite video display, you must first edit the file config.txt
- You cannot use both an HDMI and composite display at the same time


Your Pi will now boot into NOOBS and should display a list of operating systems that you can choose to install.
If your display remains blank, you should select the correct output mode for your display by pressing one of the following number keys on your keyboard:
1. HDMI mode - this is the default display mode.
2. HDMI safe mode - select this mode if you are using the HDMI connector and cannot see anything on screen when the Pi has booted.
3. Composite PAL mode - select either this mode or composite NTSC mode if you are using a screen connected to the composite video output. On most Raspberry Pis the composite video output is on the 4-pole 3.5mm jack, along with stereo audio. On the original Raspberry Pi, released in 2012, composite video is output on the RCA connector. Not all 4-pole 3.5mm cables are suitable for using to connect composite video; see the [Monitor Connection](https://www.raspberrypi.org/documentation/setup/monitor-connection.md) page for details of which cable to use.
4. Composite NTSC mode

If you are still having difficulties after following these instructions, you can ask for help on the Raspberry Pi forums at https://www.raspberrypi.org/forums .
