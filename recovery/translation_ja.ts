<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ja" sourcelanguage="en">
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
        <translation type="unfinished">エラー</translation>
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
        <translation>イメージをSDカードへ書込み中</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="63"/>
        <source>Resizing file system</source>
        <translation>ファイルシステムのサイズ変更中</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="67"/>
        <source>Patching /boot/cmdline.txt</source>
        <translation>/boot /cmdline.txt へのパッチ適用中</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="71"/>
        <source>Patching /etc/fstab</source>
        <translation>/etc/fstab へのパッチ適用中</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="75"/>
        <location filename="imagewritethread.cpp" line="107"/>
        <source>Finish writing (sync)</source>
        <translation>書き込み完了(同期)</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="202"/>
        <source>Error writing image to SD card</source>
        <translation>SDカードへの書き込みエラー</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="268"/>
        <location filename="imagewritethread.cpp" line="452"/>
        <source>Error mounting ext4 partition</source>
        <translation>ext4パーティションのマウントエラー</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="282"/>
        <source>Error mounting FAT partition of image</source>
        <translation>イメージのFATパーティションのマウントエラー</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="375"/>
        <source>No partitions found inside image</source>
        <translation>イメージ内にパーティションが見つかりません</translation>
    </message>
    <message>
        <location filename="imagewritethread.cpp" line="464"/>
        <source>Error resizing ext4 partition</source>
        <translation>ext4パーティションのリサイズエラー</translation>
    </message>
</context>
<context>
    <name>InitDriveThread</name>
    <message>
        <location filename="initdrivethread.cpp" line="41"/>
        <source>SD card too small. Must be at least %1 MB</source>
        <translation>SDカードの容量が足りません。少なくとも %1 MB 必要です</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="45"/>
        <location filename="initdrivethread.cpp" line="276"/>
        <source>Mounting FAT partition</source>
        <translation>FATパーティションをマウントしています</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="133"/>
        <source>Saving boot files to memory</source>
        <translation>のブートファイルをSDメモリに保存しています</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="136"/>
        <source>Error saving boot files to memory. SD card may be damaged.</source>
        <translation>SDカードへのブートファイル保存エラー。SDカードは壊れているかもしれません。</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="141"/>
        <location filename="initdrivethread.cpp" line="180"/>
        <source>Error unmounting system partition.</source>
        <translation>システムパーティションのマウント解除エラー.</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="145"/>
        <source>Zeroing partition table</source>
        <translation>パーティションテーブルのクリア中</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="148"/>
        <source>Error zero&apos;ing MBR/GPT. SD card may be broken or advertising wrong capacity.</source>
        <translation>MBR/GPTクリア時のエラー。 SDカードは壊れているか、間違った容量を報告しています。</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="152"/>
        <source>Creating partitions</source>
        <translation>パーティションの作成中</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="156"/>
        <source>Error partitioning</source>
        <translation>パーティション分割エラー</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="160"/>
        <source>Formatting boot partition (fat)</source>
        <translation>ブートパーティション(FAT)のフォーマット中</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="163"/>
        <source>Error formatting boot partition (fat)</source>
        <translation>ブートパーティション(FAT)のフォーマットエラー</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="167"/>
        <source>Copying boot files to storage</source>
        <translation>ブートファイルをストレージへコピーしています</translation>
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
        <translation>cmdline.txt を編集しています</translation>
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
        <translation>ブートパーティションをマウント解除しています</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="105"/>
        <source>Finish writing to disk (sync)</source>
        <translation>ディスクへの書き込みが完了しました（同期）</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="115"/>
        <source>Mounting boot partition again</source>
        <translation>ブートパーティションを再びマウントしています</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="124"/>
        <source>SD card broken (writes do not persist)</source>
        <translation>SDカードが壊れています（書込みが保持されません）</translation>
    </message>
</context>
<context>
    <name>LanguageDialog</name>
    <message>
        <location filename="languagedialog.ui" line="86"/>
        <source>Dialog</source>
        <translation>ダイアログ</translation>
    </message>
    <message>
        <location filename="languagedialog.ui" line="106"/>
        <source>Language (L):  </source>
        <translation type="unfinished">言語: </translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.ui" line="95"/>
        <source>Install OS</source>
        <translation>イメージの復元</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="98"/>
        <source>Installs the selected operating system image onto this SD card.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="101"/>
        <source>Return</source>
        <translation>リターン</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="110"/>
        <source>Exit (Esc)</source>
        <translation>終了(Esc)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="113"/>
        <source>Exits and boots the installed operating system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="116"/>
        <source>Esc</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="128"/>
        <source>Advanced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="137"/>
        <source>Add image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="149"/>
        <source>Remove image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="161"/>
        <source>Edit config (e)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="164"/>
        <source>Opens the config.txt and cmdline.txt for the installed OS in a text editor.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="167"/>
        <source>e</source>
        <translation>e</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="176"/>
        <source>Online help (h)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="179"/>
        <source>Opens a web browser displaying the Raspberry Pi Forum.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="182"/>
        <source>h</source>
        <translation>h</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="107"/>
        <location filename="mainwindow.cpp" line="318"/>
        <source>[RECOMMENDED]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="226"/>
        <source>Confirm</source>
        <translation>確認</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="43"/>
        <source>NOOBS v%1 - Built: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="54"/>
        <source>Setting up SD card</source>
        <translation type="unfinished">SDカードのセットアップ</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="93"/>
        <source>No OS Images Found on SD Card</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="93"/>
        <source>Please add at least one OS image to the /images directory in order to proceed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="227"/>
        <source>Warning: this will install the selected Operating System. All existing data on the SD card will be overwritten.</source>
        <translation>本当にイメージを書き込んでもよいですか?この操作で既存のすべてのデータは削除されます!</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="292"/>
        <source>Restore complete</source>
        <translation>リストア完了</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="293"/>
        <source>Image applied successfully</source>
        <translation>イメージは正常に適用されました</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="301"/>
        <source>Error</source>
        <translation>エラー</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="384"/>
        <source>Enabling network interface</source>
        <translation type="unfinished">ネットワーク·インターフェースを有効にしています</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="404"/>
        <source>No network access</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="405"/>
        <source>Wired network access is required for this feature. Please insert a network cable into the network port.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ProgressSlideshowDialog</name>
    <message>
        <location filename="progressslideshowdialog.cpp" line="113"/>
        <source>%1 MB of %2 MB written (%3 MB/sec)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="progressslideshowdialog.cpp" line="118"/>
        <source>%1 MB written (%2 MB/sec)</source>
        <translation type="unfinished"></translation>
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
        <location filename="languagedialog.cpp" line="25"/>
        <source>Cancel</source>
        <translation>キャンセル</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="26"/>
        <source>&amp;Cancel</source>
        <translation>キャンセル(&amp;C)</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="27"/>
        <source>Close</source>
        <translation>閉じる</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="28"/>
        <source>&amp;Close</source>
        <translation>閉じる(&amp;C)</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="29"/>
        <source>&amp;Yes</source>
        <translation>はい(&amp;Y)</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="30"/>
        <source>&amp;No</source>
        <translation>いいえ(&amp;N)</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="24"/>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
</context>
</TS>
