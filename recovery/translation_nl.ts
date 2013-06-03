<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="nl" sourcelanguage="en">
<context>
    <name>ConfEditDialog</name>
    <message>
        <location filename="confeditdialog.ui" line="22"/>
        <source>Config editor</source>
        <translation>Configuratie</translation>
    </message>
    <message>
        <location filename="confeditdialog.cpp" line="81"/>
        <source>Error</source>
        <translation>Fout</translation>
    </message>
    <message>
        <location filename="confeditdialog.cpp" line="82"/>
        <source>Error mounting boot partition</source>
        <translation>Fout bij mounten opstart partitie</translation>
    </message>
</context>
<context>
    <name>ImageWriteThread</name>
    <message>
        <location filename="imagewritethread.cpp" line="59"/>
        <location filename="imagewritethread.cpp" line="88"/>
        <source>Writing image to SD card</source>
        <translation>Bezig met wegschrijven van image</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="63"/>
        <source>Resizing file system</source>
        <translation>Vergroten van bestandssysteem</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="67"/>
        <source>Patching /boot/cmdline.txt</source>
        <translation>Aanpassen /boot/cmdline.txt</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="71"/>
        <source>Patching /etc/fstab</source>
        <translation>Aanpassen /etc/fstab</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="75"/>
        <location filename="imagewritethread.cpp" line="107"/>
        <source>Finish writing (sync)</source>
        <translation>Bezig met wegschrijven (sync)</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="92"/>
        <source>Creating FAT partition for RiscOS</source>
        <translation>Aanmaken FAT partitie voor RiscOS</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="171"/>
        <source>Image file corrupt</source>
        <translation>Image bestand beschadigd</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="164"/>
        <source>Unknown image format file extension. Expecting .lzo, .gz, .xz, .bz2 or .zip</source>
        <translation>Onbekend image formaat. Ondersteunde formaten: .lzo, .gz, .xz, .bz2 en .zip</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="202"/>
        <source>Error writing image to SD card</source>
        <translation>Fout bij schrijven naar SD card</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="268"/>
        <location filename="imagewritethread.cpp" line="452"/>
        <source>Error mounting ext4 partition</source>
        <translation>Fout bij mounten ext4 partitie</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="282"/>
        <source>Error mounting FAT partition of image</source>
        <translation>Fout bij mounten FAT partition van image</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="375"/>
        <source>No partitions found inside image</source>
        <translation>Geen partities gevonden in image</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="464"/>
        <source>Error resizing ext4 partition</source>
        <translation>Fout bij vergroten ext4 partitie</translation>
    </message>
</context>
<context>
    <name>InitDriveThread</name>
    <message>
        <location filename="initdrivethread.cpp" line="41"/>
        <source>SD card too small. Must be at least %1 MB</source>
        <translation>SD kaart te klein. Moet minimaal %1 MB zijn</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="45"/>
        <location filename="initdrivethread.cpp" line="276"/>
        <source>Mounting FAT partition</source>
        <translation>Mounten FAT partitie</translation>
    </message>
    <message>
        <source>SD card contains extra files that do not belong to this distribution. Please copy them to another disk and delete them from card.</source>
        <translation type="obsolete">SD kaart bevat extra bestanden die niet bij deze software horen. Kopieer deze naar een andere schijf en verwijder deze.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="133"/>
        <source>Saving boot files to memory</source>
        <translation>Opslaan van opstartbestanden in geheugen</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="136"/>
        <source>Error saving boot files to memory. SD card may be damaged.</source>
        <translation>Fout bij opslaan boot bestanden naar geheugen. SD kaart mogelijk kapot.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="141"/>
        <location filename="initdrivethread.cpp" line="180"/>
        <source>Error unmounting system partition.</source>
        <translation>Fout bij unmounten systeem partitie.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="145"/>
        <source>Zeroing partition table</source>
        <translation>Wissen partitie tabel</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="148"/>
        <source>Error zero&apos;ing MBR/GPT. SD card may be broken or advertising wrong capacity.</source>
        <translation>Fout bij wissen MBR/GPT. SD kaart kan kapot zijn.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="152"/>
        <source>Creating partitions</source>
        <translation>Aanmaken partities</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="156"/>
        <source>Error partitioning</source>
        <translation>Fout bij partitioneren</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="160"/>
        <source>Formatting boot partition (fat)</source>
        <translation>Formateren opstart partitie (fat)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="163"/>
        <source>Error formatting boot partition (vfat)</source>
        <translation>Fout bij formateren boot partitie (vfat)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="167"/>
        <source>Copying boot files to storage</source>
        <translation>Kopieeren van opstart bestanden</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="186"/>
        <source>SD card does not have a MBR. Cannot resize FAT partition.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="190"/>
        <source>Removing partitions 2,3,4</source>
        <translation>Verwijderen van bestaande partities 2,3,4</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="204"/>
        <source>Resizing FAT partition</source>
        <translation>Bezig met vergroten van FAT partitie</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="216"/>
        <source>Error resizing existing FAT partition</source>
        <translation></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="223"/>
        <source>Creating extended partition</source>
        <translation>Aanmaken extended partitie</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="258"/>
        <source>Error creating extended partition</source>
        <translation></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="279"/>
        <source>Error mounting system partition.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="69"/>
        <source>Editing cmdline.txt</source>
        <translation>Aanpassen cmdline.txt</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="79"/>
        <source>Error opening %1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="92"/>
        <source>Writing RiscOS blob</source>
        <translation></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="95"/>
        <source>Error writing RiscOS blob</source>
        <translation></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="102"/>
        <source>Unmounting boot partition</source>
        <translation>Unmounten opstart partitie</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="105"/>
        <source>Finish writing to disk (sync)</source>
        <translation>Bezig met wegschrijven (sync)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="115"/>
        <source>Mounting boot partition again</source>
        <translation>Mounten opstart partitie</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="124"/>
        <source>SD card broken (writes do not persist)</source>
        <translation>SD kaart kapot (writes do not persist)</translation>
    </message>
</context>
<context>
    <name>LanguageDialog</name>
    <message>
        <location filename="languagedialog.ui" line="86"/>
        <source>Dialog</source>
        <translation></translation>
    </message>
    <message>
        <location filename="languagedialog.ui" line="106"/>
        <source>Language:  </source>
        <translation>Taal:</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.ui" line="19"/>
        <source>System recovery</source>
        <translation>Systeem herstel</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="86"/>
        <location filename="mainwindow.ui" line="89"/>
        <source>Install OS</source>
        <translation>Installeer</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="92"/>
        <source>Return</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="101"/>
        <source>Exit</source>
        <translation>Afsluiten</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="104"/>
        <source>Esc</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="116"/>
        <source>Advanced</source>
        <translation>Geavanceerd</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="125"/>
        <source>Add image</source>
        <translation>Image toevoegen</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="137"/>
        <source>Remove image</source>
        <translation>Image verwijderen</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="149"/>
        <source>Edit config</source>
        <translation>Wijzig config</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="158"/>
        <location filename="mainwindow.ui" line="161"/>
        <source>Online help</source>
        <translation>Online help</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="105"/>
        <source>[RECOMMENDED]</source>
        <translation>[AANBEVOLEN]</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="211"/>
        <source>Confirm</source>
        <translation>Bevestigen</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="40"/>
        <source>Pi Recovery - Built:%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="52"/>
        <source>Setting up SD card</source>
        <translation type="unfinished">SD card voorbereiden</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="91"/>
        <source>No OS Images Found on SD Card</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="91"/>
        <source>Please add at least one OS image to the /images directory in order to proceed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="212"/>
        <source>Warning: this will install the selected Operating System. All existing data on the SD card will be overwritten.</source>
        <translation>Weet u zeker dat u de image wilt installeren?
Hierbij worden alle bestaande bestanden verwijderd.</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="245"/>
        <source>Starting image write</source>
        <translation>Bezig met wegschrijven image</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="272"/>
        <source>Restore complete</source>
        <translation>Klaar met herstellen</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="273"/>
        <source>Image applied successfully</source>
        <translation>Image successvol weggeschreven</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="281"/>
        <source>Error</source>
        <translation>Fout</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="346"/>
        <source>Enabling network interface</source>
        <translation>Activeren netwerk verbinding</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="366"/>
        <source>No network access</source>
        <translation>Geen netwerk verbinding</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="367"/>
        <source>Wired network access is required for this feature. Please insert a network cable into the network port.</source>
        <translation>Een bekabelde netwerk verbinding is vereist voor deze functionaliteit. Controleer of de netwerk kabel aangesloten zit.</translation>
    </message>
</context>
<context>
    <name>ProgressSlideshowDialog</name>
    <message>
        <location filename="progressslideshowdialog.cpp" line="104"/>
        <source>%1 MB of %2 MB written (%3 MB/sec)</source>
        <translation>%1 MB van %2 MB weggeschreven (%3 MB/sec)</translation>
    </message>
    <message>
        <location filename="progressslideshowdialog.cpp" line="109"/>
        <source>%1 MB written (%2 MB/sec)</source>
        <translation>%1 MB weggeschreven (%2 MB/sec)</translation>
    </message>
</context>
<context>
    <name>QDialogButtonBox</name>
    <message>
        <location filename="languagedialog.cpp" line="22"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="23"/>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="24"/>
        <source>Cancel</source>
        <translation>Annuleren</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="25"/>
        <source>&amp;Cancel</source>
        <oldsource>&amp;amp;Cancel</oldsource>
        <translation>&amp;Annuleren</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="27"/>
        <source>&amp;Close</source>
        <oldsource>&amp;amp;Close</oldsource>
        <translation>&amp;Sluiten</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="26"/>
        <source>Close</source>
        <translation>Sluiten</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="28"/>
        <source>&amp;Yes</source>
        <oldsource>&amp;amp;Yes</oldsource>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="29"/>
        <source>&amp;No</source>
        <translation>&amp;Nee</translation>
    </message>
</context>
</TS>
