<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="de" sourcelanguage="en">
<context>
    <name>ConfEditDialog</name>
    <message>
        <location filename="confeditdialog.ui" line="22"/>
        <source>Config editor</source>
        <translation>Konfigurationseditor</translation>
    </message>
    <message>
        <location filename="confeditdialog.cpp" line="82"/>
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="confeditdialog.cpp" line="83"/>
        <source>Error mounting boot partition</source>
        <translation>Fehler beim Einbinden der Bootpartition</translation>
    </message>
</context>
<context>
    <name>ImageWriteThread</name>
    <message>
        <location filename="imagewritethread.cpp" line="57"/>
        <location filename="imagewritethread.cpp" line="92"/>
        <source>Writing image to SD card</source>
        <translation>Das Image wird auf die SD-Karte geschrieben</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="61"/>
        <source>Resizing file system</source>
        <translation>Die Größe des Dateisystems wird angepasst</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="65"/>
        <source>Patching /boot/cmdline.txt</source>
        <translation>/boot/cmdline.txt wird gepatcht</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="69"/>
        <source>Patching /etc/fstab</source>
        <translation>/etc/fstab wird gepatcht</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="73"/>
        <location filename="imagewritethread.cpp" line="111"/>
        <source>Finish writing (sync)</source>
        <translation>Das Schreiben wird beendet (sync)</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="88"/>
        <source>RISCOS cannot be installed. Size of rescue partition too large.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="96"/>
        <source>Creating FAT partition for RiscOS</source>
        <translation>Erzeuge FAT-Partition für RiscOS</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="175"/>
        <source>Image file corrupt</source>
        <translation>Image-Datei beschädigt bzw. ungültig</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="360"/>
        <source>Extended boot record (EBR) not found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="375"/>
        <source>No partitions found inside image&apos;s extended boot record (EBR)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="390"/>
        <source>Logical boot record (LBR) not found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="405"/>
        <source>No partitions found inside image&apos;s logical boot record (LBR)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="168"/>
        <source>Unknown image format file extension. Expecting .lzo, .gz, .xz, .bz2 or .zip</source>
        <translation>Die Dateierweiterung der Imagedatei ist unbekannt. Unterstüzt werden .lzo .gz, .xz, .bz2 und .zip</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="206"/>
        <source>Error writing image to SD card</source>
        <translation>Fehler beim Schreiben des Image auf die SD-Karte</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="272"/>
        <location filename="imagewritethread.cpp" line="484"/>
        <source>Error mounting ext4 partition</source>
        <translation>Fehler beim Mounten der ext4-Partition</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="286"/>
        <source>Error mounting FAT partition of image</source>
        <translation>Fehler beim Mounten der FAT-Partition im Image</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="496"/>
        <source>Error resizing ext4 partition</source>
        <translation>Fehler beim Anpassen der Größe der ext4-Partition</translation>
    </message>
</context>
<context>
    <name>InitDriveThread</name>
    <message>
        <location filename="initdrivethread.cpp" line="39"/>
        <source>SD card too small. Must be at least %1 MB</source>
        <translation>Die SD-Karte ist zu klein, sie muss mindestens %1 MB groß sein</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="43"/>
        <location filename="initdrivethread.cpp" line="325"/>
        <source>Mounting FAT partition</source>
        <translation>Die FAT-Partition wird gemountet</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="131"/>
        <source>Saving boot files to memory</source>
        <translation>Die Boot-Dateien werden im Hauptspeicher gesichert</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="134"/>
        <source>Error saving boot files to memory. SD card may be damaged.</source>
        <translation>Fehler beim Speichern der Boot-Dateien, die SD-Karte ist möglicherweise beschädigt.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="139"/>
        <location filename="initdrivethread.cpp" line="179"/>
        <source>Error unmounting system partition.</source>
        <translation>Fehler beim Abmelden der Systempartition.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="143"/>
        <location filename="initdrivethread.cpp" line="197"/>
        <source>Zeroing partition table</source>
        <translation>Die Partitionstabelle wird gelöscht</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="146"/>
        <location filename="initdrivethread.cpp" line="200"/>
        <source>Error zero&apos;ing MBR/GPT. SD card may be broken or advertising wrong capacity.</source>
        <translation>Fehler beim Löschen der Partitionstabelle (MBR/GPT). Die SD-Karte ist möglicherweise beschädigt oder meldet eine falsche Kapazität.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="150"/>
        <source>Creating partitions</source>
        <translation>Die Partitionen werden angelegt</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="154"/>
        <source>Error partitioning</source>
        <translation>Fehler beim Partitionieren</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="158"/>
        <source>Formatting boot partition (fat)</source>
        <translation>Die Boot-Partition wird als FAT-Dateisystem formatiert</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="161"/>
        <source>Error formatting boot partition (fat)</source>
        <translation>Fehler beim Formatieren der Boot-Partition</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="165"/>
        <source>Copying boot files to storage</source>
        <translation>Die Boot-Dateien werden kopiert</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="190"/>
        <source>Would you like NOOBS to create one for you?
WARNING: This will erase all data on your SD card</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="191"/>
        <source>Error: No MBR present on SD Card</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="205"/>
        <source>Writing new MBR</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="213"/>
        <source>Error creating MBR</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="221"/>
        <source>SD card has now been formatted ready for NOOBS installation. Please re-copy the NOOBS files onto the card and reboot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="226"/>
        <source>SD card has not been formatted correctly. Please reformat using the SD Association Formatting Tool and try again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="232"/>
        <source>Removing partitions 2,3,4</source>
        <translation>Entferne Partitionen 2,3,4</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="246"/>
        <source>Resizing FAT partition</source>
        <translation>Bestehende FAT-Partition in Größe verändern</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="265"/>
        <source>Error resizing existing FAT partition</source>
        <translation>Fehler bei Größenänderung der bestehenden FAT-Partition</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="272"/>
        <source>Creating extended partition</source>
        <translation>Lege Erweiterte Partition an</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="307"/>
        <source>Error creating extended partition</source>
        <translation>Fehler beim Anlegen der Erweiterten Partition</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="328"/>
        <source>Error mounting system partition.</source>
        <translation>Fehler beim Einbinden der Systempartition</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="67"/>
        <source>Editing cmdline.txt</source>
        <translation>cmdline.txt bearbeiten</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="77"/>
        <source>Error opening %1</source>
        <translation>Fehler beim Öffnen von %1</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="90"/>
        <source>Writing RiscOS blob</source>
        <translation>Schreibe RiscOS blob</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="93"/>
        <source>Error writing RiscOS blob</source>
        <translation>Fehler beim Schreiben des RiscOS blob</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="100"/>
        <source>Unmounting boot partition</source>
        <translation>Die Boot-Partition wird abgemeldet</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="103"/>
        <source>Finish writing to disk (sync)</source>
        <translation>Das Schreiben wird beendet (sync)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="113"/>
        <source>Mounting boot partition again</source>
        <translation>Die Boot-Partition wird erneut gemountet</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="122"/>
        <source>SD card broken (writes do not persist)</source>
        <translation>Die SD-Karte ist defekt (geschriebene Daten bleiben nicht erhalten)</translation>
    </message>
</context>
<context>
    <name>LanguageDialog</name>
    <message>
        <location filename="languagedialog.ui" line="86"/>
        <source>Dialog</source>
        <translation>Dialog</translation>
    </message>
    <message>
        <location filename="languagedialog.ui" line="106"/>
        <source>Language (l): </source>
        <translation>Sprache (l): </translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.ui" line="95"/>
        <source>Install OS</source>
        <translation>System installieren</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="98"/>
        <source>Installs the selected operating system image onto this SD card.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="107"/>
        <source>Exit (Esc)</source>
        <translation>Beenden (Esc)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="110"/>
        <source>Exits and boots the installed operating system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="125"/>
        <source>Advanced</source>
        <translation>Erweitert</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="134"/>
        <source>Add image</source>
        <translation>System hinzufügen</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="146"/>
        <source>Remove image</source>
        <translation>System ertfernen</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="158"/>
        <source>Edit config (e)</source>
        <translation>Einstellungen bearbeiten (e)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="161"/>
        <source>Opens the config.txt and cmdline.txt for the installed OS in a text editor.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="173"/>
        <source>Online help (h)</source>
        <translation>Online-Hilfe (h)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="176"/>
        <source>Opens a web browser displaying the Raspberry Pi Forum.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="349"/>
        <source>RECOMMENDED</source>
        <translation>EMPFOHLEN</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="241"/>
        <source>Confirm</source>
        <translation>Bestätigung</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="342"/>
        <source>NOOBS v%1 - Built: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="66"/>
        <source>Setting up SD card</source>
        <translation>Die SD-Karte wird eingerichtet</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="108"/>
        <source>No OS Images Found on SD Card</source>
        <translation>Kein Betriebssystem auf der SD-Karte gefunden</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="108"/>
        <source>Please add at least one OS image to the /images directory in order to proceed</source>
        <translation>Bitte fügen Sie mind. ein Betriebssystem dem Ordner /images hinzu um fortzufahren</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="242"/>
        <source>Warning: this will install the selected Operating System. All existing data on the SD card will be overwritten.</source>
        <translation>Bist Du sicher, dass Du das Image schreiben möchtest? ALLE DATEN AUF DER SD-KARTE GEHEN VERLOREN!</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="314"/>
        <source>Restore complete</source>
        <translation>Wiederherstellung vollständing</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="315"/>
        <source>Image applied successfully</source>
        <translation>Image erfolgreich wiederhergestellt</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="323"/>
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="400"/>
        <source>Display Mode Changed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="401"/>
        <source>Display mode changed to %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="428"/>
        <source>HDMI preferred mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="434"/>
        <source>HDMI safe mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="440"/>
        <source>composite PAL mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="446"/>
        <source>composite NTSC mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="491"/>
        <source>Enabling network interface</source>
        <translation>Die Netzwerkschnittstelle wird aktiviert</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="511"/>
        <source>No network access</source>
        <translation>Kein Netzwerk gefunden</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="512"/>
        <source>Wired network access is required for this feature. Please insert a network cable into the network port.</source>
        <translation>Hierfür ist eine kabelgebundene Netzwerkverbindung erforderlich. Bitte stecken Sie ein Netzwerkkabel ein.</translation>
    </message>
</context>
<context>
    <name>ProgressSlideshowDialog</name>
    <message>
        <location filename="progressslideshowdialog.cpp" line="111"/>
        <source>%1 MB of %2 MB written (%3 MB/sec)</source>
        <translation>%1 MB von %2 MB (%3 MB/Sek)</translation>
    </message>
    <message>
        <location filename="progressslideshowdialog.cpp" line="116"/>
        <source>%1 MB written (%2 MB/sec)</source>
        <translation>%1 MB geschrieben (%2 MB/Sek)</translation>
    </message>
</context>
<context>
    <name>QDialogButtonBox</name>
    <message>
        <location filename="languagedialog.cpp" line="24"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="26"/>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="27"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="28"/>
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="29"/>
        <source>&amp;Close</source>
        <translation>Schl&amp;ießen</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="30"/>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="31"/>
        <source>&amp;No</source>
        <translation>&amp;Nein</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="25"/>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
</context>
</TS>
