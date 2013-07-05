<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="fr" sourcelanguage="en">
<context>
    <name>ConfEditDialog</name>
    <message>
        <location filename="confeditdialog.ui" line="22"/>
        <source>Config editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="confeditdialog.cpp" line="82"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="confeditdialog.cpp" line="83"/>
        <source>Error mounting boot partition</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ImageWriteThread</name>
    <message>
        <location filename="imagewritethread.cpp" line="57"/>
        <location filename="imagewritethread.cpp" line="92"/>
        <source>Writing image to SD card</source>
        <translation>Écriture de l&apos;image sur la carte SD</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="61"/>
        <source>Resizing file system</source>
        <translation>Redimensionnement du système de fichiers</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="65"/>
        <source>Patching /boot/cmdline.txt</source>
        <translation>Application du patch sur /boot/cmdline.txt</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="69"/>
        <source>Patching /etc/fstab</source>
        <translation>Application du patch sur /etc/fstab</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="73"/>
        <location filename="imagewritethread.cpp" line="111"/>
        <source>Finish writing (sync)</source>
        <translation>Écriture terminée (sync)</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="88"/>
        <source>RISCOS cannot be installed. Size of rescue partition too large.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="96"/>
        <source>Creating FAT partition for RiscOS</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="175"/>
        <source>Image file corrupt</source>
        <translation type="unfinished"></translation>
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
        <translation>Extension du fichier image inconnue. Extension attendue .gz, .xz, .bz2 ou .zip</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="206"/>
        <source>Error writing image to SD card</source>
        <translation>Erreur d&apos;écriture de l&apos;image sur la carte SD</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="272"/>
        <location filename="imagewritethread.cpp" line="484"/>
        <source>Error mounting ext4 partition</source>
        <translation>Erreur de montage de la partition ext4</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="286"/>
        <source>Error mounting FAT partition of image</source>
        <translation>Erreur de montage de la partition FAT de l&apos;image</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="496"/>
        <source>Error resizing ext4 partition</source>
        <translation>Erreur de redimensionnement de la partition ext4</translation>
    </message>
</context>
<context>
    <name>InitDriveThread</name>
    <message>
        <location filename="initdrivethread.cpp" line="39"/>
        <source>SD card too small. Must be at least %1 MB</source>
        <translation>Carte SD trop petite. Taille nécessaire d&apos;au moins %1 M</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="43"/>
        <location filename="initdrivethread.cpp" line="325"/>
        <source>Mounting FAT partition</source>
        <translation>Montage de la partition FAT</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="131"/>
        <source>Saving boot files to memory</source>
        <translation>nregistrement des fichiers de démarrage sur la carte SD</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="134"/>
        <source>Error saving boot files to memory. SD card may be damaged.</source>
        <translation>rreur pendant l&apos;enregistrement des fichiers sur la carte. La carte SD est peut-être endommagée.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="139"/>
        <location filename="initdrivethread.cpp" line="179"/>
        <source>Error unmounting system partition.</source>
        <translation>Erreur pendant le montage de la partition système.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="143"/>
        <location filename="initdrivethread.cpp" line="197"/>
        <source>Zeroing partition table</source>
        <translation>Initialisation de la table de partitions</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="146"/>
        <location filename="initdrivethread.cpp" line="200"/>
        <source>Error zero&apos;ing MBR/GPT. SD card may be broken or advertising wrong capacity.</source>
        <translation>Erreur pendant l&apos;initialisation MBR/GPT. La carte SD est peut-être endommagée ou annonce une mauvaise capacité.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="150"/>
        <source>Creating partitions</source>
        <translation>Création des partitions</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="154"/>
        <source>Error partitioning</source>
        <translation>Erreur lors de la création des partitions</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="158"/>
        <source>Formatting boot partition (fat)</source>
        <translation>Formatage de la partition de démarrage (FAT)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="161"/>
        <source>Error formatting boot partition (fat)</source>
        <translation>Erreur pendant le formatage de la partition de démarrage (FAT)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="165"/>
        <source>Copying boot files to storage</source>
        <translation>Copie en cours des fichiers de démarrage sur la carte SD</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="246"/>
        <source>Resizing FAT partition</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="265"/>
        <source>Error resizing existing FAT partition</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="272"/>
        <source>Creating extended partition</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="307"/>
        <source>Error creating extended partition</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="328"/>
        <source>Error mounting system partition.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="67"/>
        <source>Editing cmdline.txt</source>
        <translation>Modification du fichier cmdline.txt</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="77"/>
        <source>Error opening %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="90"/>
        <source>Writing RiscOS blob</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="93"/>
        <source>Error writing RiscOS blob</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="100"/>
        <source>Unmounting boot partition</source>
        <translation>Démontage de la partition de démarrage</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="103"/>
        <source>Finish writing to disk (sync)</source>
        <translation>Écriture sur la carte SD en cours de finalisation (sync)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="113"/>
        <source>Mounting boot partition again</source>
        <translation>Remontage de la partition de démarrage</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="122"/>
        <source>SD card broken (writes do not persist)</source>
        <translation>Carte SD défaillante (écriture non persistante)</translation>
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
        <source>Language (l): </source>
        <translation>Langue (l): </translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.ui" line="95"/>
        <source>Install OS</source>
        <translation>Restaurer image</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="98"/>
        <source>Installs the selected operating system image onto this SD card.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="107"/>
        <source>Exit (Esc)</source>
        <translation>Sortie (Esc)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="110"/>
        <source>Exits and boots the installed operating system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="125"/>
        <source>Advanced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="134"/>
        <source>Add image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="146"/>
        <source>Remove image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="158"/>
        <source>Edit config (e)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="161"/>
        <source>Opens the config.txt and cmdline.txt for the installed OS in a text editor.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="173"/>
        <source>Online help (h)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="176"/>
        <source>Opens a web browser displaying the Raspberry Pi Forum.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="342"/>
        <source>NOOBS v%1 - Built: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="66"/>
        <source>Setting up SD card</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="108"/>
        <source>No OS Images Found on SD Card</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="108"/>
        <source>Please add at least one OS image to the /images directory in order to proceed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="349"/>
        <source>RECOMMENDED</source>
        <translation>RECOMMANDÉ</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="241"/>
        <source>Confirm</source>
        <translation>Confirmer</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="242"/>
        <source>Warning: this will install the selected Operating System. All existing data on the SD card will be overwritten.</source>
        <translation>Êtes-vous sûr de vouloir écrire l&apos;image? CELA EFFACERA TOUTES VOS DONNÉES EXISTANTES!</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="314"/>
        <source>Restore complete</source>
        <translation>Restauration complète</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="315"/>
        <source>Image applied successfully</source>
        <translation>Écriture de l’image réussite</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="323"/>
        <source>Error</source>
        <translation>Erreur</translation>
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
        <translation>Activation de l’interface réseau</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="511"/>
        <source>No network access</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="512"/>
        <source>Wired network access is required for this feature. Please insert a network cable into the network port.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ProgressSlideshowDialog</name>
    <message>
        <location filename="progressslideshowdialog.cpp" line="111"/>
        <source>%1 MB of %2 MB written (%3 MB/sec)</source>
        <translation>%1 MB sur %2 MB écrite (%3 MB/sec)</translation>
    </message>
    <message>
        <location filename="progressslideshowdialog.cpp" line="116"/>
        <source>%1 MB written (%2 MB/sec)</source>
        <translation>%1 MB écrite (%2 MB/sec) </translation>
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
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="27"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Annuler</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="28"/>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="29"/>
        <source>&amp;Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="30"/>
        <source>&amp;Yes</source>
        <translation>&amp;Oui</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="31"/>
        <source>&amp;No</source>
        <translation>&amp;Non</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="25"/>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
</context>
</TS>
