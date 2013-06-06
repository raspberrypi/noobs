<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="de" sourcelanguage="en">
<context>
    <name>ConfEditDialog</name>
    <message>
        <location filename="confeditdialog.ui" line="22"/>
        <source>Config editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="confeditdialog.cpp" line="81"/>
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="confeditdialog.cpp" line="82"/>
        <source>Error mounting boot partition</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ImageWriteThread</name>
    <message>
        <location filename="imagewritethread.cpp" line="59"/>
        <location filename="imagewritethread.cpp" line="88"/>
        <source>Writing image to SD card</source>
        <translation>Das Image wird auf die SD-Karte geschrieben</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="63"/>
        <source>Resizing file system</source>
        <translation>Die Größe des Dateisystems wird angepasst</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="67"/>
        <source>Patching /boot/cmdline.txt</source>
        <translation>/boot/cmdline.txt wird gepatcht</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="71"/>
        <source>Patching /etc/fstab</source>
        <translation>/etc/fstab wird gepatcht</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="75"/>
        <location filename="imagewritethread.cpp" line="107"/>
        <source>Finish writing (sync)</source>
        <translation>Das Schreiben wird beendet (sync)</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="92"/>
        <source>Creating FAT partition for RiscOS</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="171"/>
        <source>Image file corrupt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="164"/>
        <source>Unknown image format file extension. Expecting .lzo, .gz, .xz, .bz2 or .zip</source>
        <translation>Die Dateierweiterung der Imagedatei ist unbekannt.  Es werden .lzo .gz, .xz, .bz2 und .zip unterstützt</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="202"/>
        <source>Error writing image to SD card</source>
        <translation>Fehler beim Schreiben des Image auf die SD-Karte</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="268"/>
        <location filename="imagewritethread.cpp" line="452"/>
        <source>Error mounting ext4 partition</source>
        <translation>Fehler beim Mounten der ext4-Partition</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="282"/>
        <source>Error mounting FAT partition of image</source>
        <translation>Fehler beim Mounten der FAT-Partition im Image</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="375"/>
        <source>No partitions found inside image</source>
        <translation>Im Image konnten keine Partitionen gefunden werden</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="464"/>
        <source>Error resizing ext4 partition</source>
        <translation>Fehler beim Anpassen der Größe der ext4-Partition</translation>
    </message>
</context>
<context>
    <name>InitDriveThread</name>
    <message>
        <location filename="initdrivethread.cpp" line="41"/>
        <source>SD card too small. Must be at least %1 MB</source>
        <translation>Die SD-Karte ist zu klein, sie muss mindestens %1 MB groß sein</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="45"/>
        <location filename="initdrivethread.cpp" line="276"/>
        <source>Mounting FAT partition</source>
        <translation>Die FAT-Partition wird gemountet</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="133"/>
        <source>Saving boot files to memory</source>
        <translation>Die Boot-Dateien werden im Hauptspeicher gesicher</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="136"/>
        <source>Error saving boot files to memory. SD card may be damaged.</source>
        <translation>Fehler beim Speichern der Boot-Dateien, die SD-Karte ist möglicherweise beschädigt.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="141"/>
        <location filename="initdrivethread.cpp" line="180"/>
        <source>Error unmounting system partition.</source>
        <translation>Fehler beim Abmelden der Systempartition.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="145"/>
        <source>Zeroing partition table</source>
        <translation>Die Partitionstabelle wird gelöscht</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="148"/>
        <source>Error zero&apos;ing MBR/GPT. SD card may be broken or advertising wrong capacity.</source>
        <translation>Fehler beim Löschen der Partitionstabelle (MBR/GPT).  Die SD-Karte ist möglicherweise beschädigt oder meldet eine falsche Kapazität.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="152"/>
        <source>Creating partitions</source>
        <translation>Die Partitionen werden angelegt</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="156"/>
        <source>Error partitioning</source>
        <translation>Fehler beim Partitionieren</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="160"/>
        <source>Formatting boot partition (fat)</source>
        <translation>Die Boot-Partition wird als FAT-Dateisystem formatiert</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="163"/>
        <source>Error formatting boot partition (fat)</source>
        <translation>Fehler beim Formatieren der Boot-Partition</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="167"/>
        <source>Copying boot files to storage</source>
        <translation>Die Boot-Dateien werden kopiert</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="186"/>
        <source>SD card does not have a MBR. Cannot resize FAT partition.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="190"/>
        <source>Removing partitions 2,3,4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="204"/>
        <source>Resizing FAT partition</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="216"/>
        <source>Error resizing existing FAT partition</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="223"/>
        <source>Creating extended partition</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="258"/>
        <source>Error creating extended partition</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="279"/>
        <source>Error mounting system partition.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="69"/>
        <source>Editing cmdline.txt</source>
        <translation>cmdline.txt wird bearbeitet</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="79"/>
        <source>Error opening %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="92"/>
        <source>Writing RiscOS blob</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="95"/>
        <source>Error writing RiscOS blob</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="102"/>
        <source>Unmounting boot partition</source>
        <translation>Die Boot-Partition wird abgemeldet</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="105"/>
        <source>Finish writing to disk (sync)</source>
        <translation>Das Schreiben wird beendet (sync)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="115"/>
        <source>Mounting boot partition again</source>
        <translation>Die Boot-Partition wird erneut gemountet</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="124"/>
        <source>SD card broken (writes do not persist)</source>
        <translation>Die SD-Karte ist kaputt (geschriebene Daten blieben nicht erhalten)</translation>
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
        <translation>Sprache: </translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.ui" line="83"/>
        <location filename="mainwindow.ui" line="86"/>
        <source>Install OS</source>
        <translation>Image wiederherstellen</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="89"/>
        <source>Return</source>
        <translation>Zurück</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="98"/>
        <source>Exit</source>
        <translation>Beenden</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="101"/>
        <source>Esc</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="113"/>
        <source>Advanced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="122"/>
        <source>Add image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="134"/>
        <source>Remove image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="146"/>
        <source>Edit config</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="155"/>
        <location filename="mainwindow.ui" line="158"/>
        <source>Online help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="105"/>
        <source>[RECOMMENDED]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="223"/>
        <source>Confirm</source>
        <translation>Bestätigung</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="40"/>
        <source>Pi Recovery - Built:%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="52"/>
        <source>Setting up SD card</source>
        <translation type="unfinished">Die SD-Karte wird eingerichtet</translation>
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
        <location filename="mainwindow.cpp" line="224"/>
        <source>Warning: this will install the selected Operating System. All existing data on the SD card will be overwritten.</source>
        <translation>Bist Du sicher, dass Du das Image schreiben möchtest?  ALLE DATEN AUF DER SD-KARTE GEHEN VERLOREN!</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="289"/>
        <source>Restore complete</source>
        <translation>Wiederherstellung vollständing</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="290"/>
        <source>Image applied successfully</source>
        <translation>Image erfolgreich wiederhergestellt</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="298"/>
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="363"/>
        <source>Enabling network interface</source>
        <translation type="unfinished">Die Netzwerkschnittstelle wird aktiviert</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="383"/>
        <source>No network access</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="384"/>
        <source>Wired network access is required for this feature. Please insert a network cable into the network port.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ProgressSlideshowDialog</name>
    <message>
        <location filename="progressslideshowdialog.cpp" line="104"/>
        <source>%1 MB of %2 MB written (%3 MB/sec)</source>
        <translation>%1 MB von %2 MB (%3 MB/sec)</translation>
    </message>
    <message>
        <location filename="progressslideshowdialog.cpp" line="109"/>
        <source>%1 MB written (%2 MB/sec)</source>
        <translation type="unfinished"></translation>
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
        <location filename="languagedialog.cpp" line="24"/>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="25"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="26"/>
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="27"/>
        <source>&amp;Close</source>
        <translation>Schl&amp;ießen</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="28"/>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="29"/>
        <source>&amp;No</source>
        <translation>&amp;Nein</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="23"/>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
</context>
</TS>
