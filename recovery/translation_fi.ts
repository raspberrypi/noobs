<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="fi_FI">
<context>
    <name>ConfEditDialog</name>
    <message>
        <location filename="confeditdialog.ui" line="22"/>
        <source>Config editor</source>
        <translation>Asetusten muokkain</translation>
    </message>
    <message>
        <location filename="confeditdialog.cpp" line="81"/>
        <source>Error</source>
        <translation>Virhe</translation>
    </message>
    <message>
        <location filename="confeditdialog.cpp" line="82"/>
        <source>Error mounting boot partition</source>
        <translation>Käynnistysosiota liitettäessä tapahtui virhe</translation>
    </message>
</context>
<context>
    <name>ImageWriteThread</name>
    <message>
        <location filename="imagewritethread.cpp" line="59"/>
        <location filename="imagewritethread.cpp" line="88"/>
        <source>Writing image to SD card</source>
        <translation>Kirjoita levykuva SD muistikortille</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="63"/>
        <source>Resizing file system</source>
        <translation>Muutetaan tiedostojärjestelmän kokoa</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="67"/>
        <source>Patching /boot/cmdline.txt</source>
        <translation>Paikataan tiedostoa /boot/cmdline.txt</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="71"/>
        <source>Patching /etc/fstab</source>
        <translation>Paikataan tiedostoa /etc/fstab</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="75"/>
        <location filename="imagewritethread.cpp" line="107"/>
        <source>Finish writing (sync)</source>
        <translation>Viimeistellään kirjoitusta (synkronointi)</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="92"/>
        <source>Creating FAT partition for RiscOS</source>
        <translation>Luodaan FAT osiota RiscOS:ia varten</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="164"/>
        <source>Unknown image format file extension. Expecting .lzo, .gz, .xz, .bz2 or .zip</source>
        <translation>En tunnista levykuvan tiedostopäätettä. Tunnetut päätteet ovat: lzo, .gz, .xz, .bz2 ja .zip</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="171"/>
        <source>Image file corrupt</source>
        <translation>Levykuva on vahingoittunut</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="202"/>
        <source>Error writing image to SD card</source>
        <translation>Virhe kirjoittettaessa levykuvaa SD muistikortille</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="268"/>
        <location filename="imagewritethread.cpp" line="480"/>
        <source>Error mounting ext4 partition</source>
        <translation>Virhe liitettäessä ext4 osiota</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="282"/>
        <source>Error mounting FAT partition of image</source>
        <translation>Virhe liitettäessä FAT osiota</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="356"/>
        <source>Extended boot record (EBR) not found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="371"/>
        <source>No partitions found inside image&apos;s extended boot record (EBR)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="386"/>
        <source>Logical boot record (LBR) not found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="401"/>
        <source>No partitions found inside image&apos;s logical boot record (LBR)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="492"/>
        <source>Error resizing ext4 partition</source>
        <translation>Virhe muutettaessa ext4 osion kokoa</translation>
    </message>
</context>
<context>
    <name>InitDriveThread</name>
    <message>
        <location filename="initdrivethread.cpp" line="41"/>
        <source>SD card too small. Must be at least %1 MB</source>
        <translation>SD muistikortti on liian pieni. Muistikortilla tullee olla vähintään %1 Mt tilaa</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="45"/>
        <location filename="initdrivethread.cpp" line="297"/>
        <source>Mounting FAT partition</source>
        <translation>Liitetään FAT osiota</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="69"/>
        <source>Editing cmdline.txt</source>
        <translation>Muokataan tiedostoa cmdline.txt</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="79"/>
        <source>Error opening %1</source>
        <translation>Virhe avattaessa tiedostoa %1</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="92"/>
        <source>Writing RiscOS blob</source>
        <translation>Kirjoitetaan RiscOS tiedostoa</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="95"/>
        <source>Error writing RiscOS blob</source>
        <translation>Virhe kirjoitettaessa RiscOS tiedostoa</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="102"/>
        <source>Unmounting boot partition</source>
        <translation>Irrotetaan käynnistysosiota</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="105"/>
        <source>Finish writing to disk (sync)</source>
        <translation>Viimeistellään kirjoitusta (synkronointi)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="115"/>
        <source>Mounting boot partition again</source>
        <translation>Liitetään käynnistysosiota uudelleen</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="124"/>
        <source>SD card broken (writes do not persist)</source>
        <translation>SD muistikortti on viallinen (muutokset eivät tallennu sille)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="133"/>
        <source>Saving boot files to memory</source>
        <translation>Tallennetaan käynnistystiedostoja muistiin</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="136"/>
        <source>Error saving boot files to memory. SD card may be damaged.</source>
        <translation>Virhe tallennettaessa käynnistystiedostoja muistiin. SD muistikortti saattaa olla viallinen.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="141"/>
        <location filename="initdrivethread.cpp" line="180"/>
        <source>Error unmounting system partition.</source>
        <translation>Virhe irrotettaessa järjestelmäosiota.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="145"/>
        <source>Zeroing partition table</source>
        <translation>Nollataan osiotaulua</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="148"/>
        <source>Error zero&apos;ing MBR/GPT. SD card may be broken or advertising wrong capacity.</source>
        <translation>Virhe MBR/GPT osiotaulua nollatessa. SD muistikortti saattaa olla viallinen tai se saattaa mainostaa väärää kapsiteettia.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="152"/>
        <source>Creating partitions</source>
        <translation>Luodaan osioita</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="156"/>
        <source>Error partitioning</source>
        <translation>Virhe osioita luodessa</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="160"/>
        <source>Formatting boot partition (fat)</source>
        <translation>Alustetaan käynnistysosiota (fat)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="163"/>
        <source>Error formatting boot partition (fat)</source>
        <translation>Virhe alustettaessa käynnistysosiota (FAT)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="167"/>
        <source>Copying boot files to storage</source>
        <translation>Kopioidaan käynnistystiedostoja medialle</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="199"/>
        <source>Error creating MBR</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="207"/>
        <source>SD card has now been formatted ready for NOOBS installation. Please re-copy the NOOBS files onto the card and reboot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="211"/>
        <source>Removing partitions 2,3,4</source>
        <translation>Poistetaan osiot 2, 3 ja 4</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="225"/>
        <source>Resizing FAT partition</source>
        <translation>Muutetaan FAT osion kokoa</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="237"/>
        <source>Error resizing existing FAT partition</source>
        <translation>Virhe muutettaessa FAT osion kokoa</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="244"/>
        <source>Creating extended partition</source>
        <translation>Luodaan &apos;extended&apos; tyyppinen osio</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="279"/>
        <source>Error creating extended partition</source>
        <translation>Virhe luotaessa &apos;extended&apos; tyyppistä osiota</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="300"/>
        <source>Error mounting system partition.</source>
        <translation>Virhe liitettäessä järjestelmäosiota.</translation>
    </message>
</context>
<context>
    <name>LanguageDialog</name>
    <message>
        <location filename="languagedialog.ui" line="86"/>
        <source>Dialog</source>
        <translation>Valikko</translation>
    </message>
    <message>
        <location filename="languagedialog.ui" line="106"/>
        <source>Language (l): </source>
        <translation>Kieli (l): </translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.ui" line="95"/>
        <source>Install OS</source>
        <translation>Asenna järjestelmä</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="98"/>
        <source>Installs the selected operating system image onto this SD card.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="107"/>
        <source>Exit (Esc)</source>
        <translation>Lopeta (Esc)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="110"/>
        <source>Exits and boots the installed operating system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="125"/>
        <source>Advanced</source>
        <translation>Edistynyt</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="134"/>
        <source>Add image</source>
        <translation>Lisää levykuva</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="146"/>
        <source>Remove image</source>
        <translation>Poista levykuva</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="158"/>
        <source>Edit config (e)</source>
        <translation>Muokkaa asetuksia (e)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="161"/>
        <source>Opens the config.txt and cmdline.txt for the installed OS in a text editor.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="173"/>
        <source>Online help (h)</source>
        <translation>Verkko ohje (h)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="176"/>
        <source>Opens a web browser displaying the Raspberry Pi Forum.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="332"/>
        <source>NOOBS v%1 - Built: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="66"/>
        <source>Setting up SD card</source>
        <translation>Valmistellaan SD muistikorttia</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="105"/>
        <source>No OS Images Found on SD Card</source>
        <translation>SD muistikortilta ei löytynyt yhtään levykuvaa</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="105"/>
        <source>Please add at least one OS image to the /images directory in order to proceed</source>
        <translation>Lisää vähintään yksi levykuva /images kansioon jos haluat jatkaa</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="339"/>
        <source>RECOMMENDED</source>
        <translation>SUOSITELTU</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="238"/>
        <source>Confirm</source>
        <translation>Vahvista</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="239"/>
        <source>Warning: this will install the selected Operating System. All existing data on the SD card will be overwritten.</source>
        <translation>Varoitus: tämä asentaa valityn käyttöjärjestelmän. Koko SD muistikortti tyhjennetään enne tätä.</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="311"/>
        <source>Restore complete</source>
        <translation>Palautus on valmis</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="312"/>
        <source>Image applied successfully</source>
        <translation>Levykuva kirjoitettu onnistuneesti</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="320"/>
        <source>Error</source>
        <translation>Virhe</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="389"/>
        <source>Display Mode Changed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="390"/>
        <source>Display mode changed to %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="413"/>
        <source>HDMI preferred mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="419"/>
        <source>HDMI safe mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="425"/>
        <source>composite PAL mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="431"/>
        <source>composite NTSC mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="476"/>
        <source>Enabling network interface</source>
        <translation>Avataan verkkoyhteyttä</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="496"/>
        <source>No network access</source>
        <translation>Ei verkkoyhteyttä</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="497"/>
        <source>Wired network access is required for this feature. Please insert a network cable into the network port.</source>
        <translation>Tämä ominausuus tarvitsee langallisen verkkoyhteyden toimiakseen. Liitä verkkokaapeli verkko porttiin.</translation>
    </message>
</context>
<context>
    <name>ProgressSlideshowDialog</name>
    <message>
        <location filename="progressslideshowdialog.cpp" line="113"/>
        <source>%1 MB of %2 MB written (%3 MB/sec)</source>
        <translatorcomment>The format is bit different, but when it comes to finnish this will the most clear format in which this can be expressed</translatorcomment>
        <translation>%1/%2 Mt kirjoittuna (%3 Mt/s)</translation>
    </message>
    <message>
        <location filename="progressslideshowdialog.cpp" line="118"/>
        <source>%1 MB written (%2 MB/sec)</source>
        <translation>%1 Mt kirjoitettuna (%2 Mt/s)</translation>
    </message>
</context>
<context>
    <name>QDialogButtonBox</name>
    <message>
        <location filename="languagedialog.cpp" line="23"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="24"/>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="25"/>
        <source>Cancel</source>
        <translation>Peruuta</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="26"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Peruuta</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="27"/>
        <source>Close</source>
        <translation>Sulje</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="28"/>
        <source>&amp;Close</source>
        <translation>&amp;Sulje</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="29"/>
        <source>&amp;Yes</source>
        <translation>&amp;Kyllä</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="30"/>
        <source>&amp;No</source>
        <translation>&amp;Ei</translation>
    </message>
</context>
</TS>
