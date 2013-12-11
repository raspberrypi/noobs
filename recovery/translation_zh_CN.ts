<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="zh_CN" sourcelanguage="en">
<context>
    <name>BootSelectionDialog</name>
    <message>
        <location filename="bootselectiondialog.ui" line="19"/>
        <location filename="bootselectiondialog.cpp" line="249"/>
        <source>Select OS to boot</source>
        <translation>选择要启动的操作系统</translation>
    </message>
    <message>
        <location filename="bootselectiondialog.cpp" line="43"/>
        <source>Cannot display boot menu</source>
        <translation>不能显示启动菜单</translation>
    </message>
    <message>
        <location filename="bootselectiondialog.cpp" line="43"/>
        <source>Error mounting settings partition</source>
        <translation>挂截设置分区出错</translation>
    </message>
    <message>
        <location filename="bootselectiondialog.cpp" line="190"/>
        <source>HDMI safe mode</source>
        <translation>HDMI 安全模式</translation>
    </message>
    <message>
        <location filename="bootselectiondialog.cpp" line="194"/>
        <source>composite PAL mode</source>
        <translation>复合 PAL 模式</translation>
    </message>
    <message>
        <location filename="bootselectiondialog.cpp" line="198"/>
        <source>composite NTSC mode</source>
        <translation>复合 NTSC 模式</translation>
    </message>
    <message>
        <location filename="bootselectiondialog.cpp" line="254"/>
        <source>Previously selected OS will boot in %1 seconds</source>
        <translation>将在 %1 秒后启动上次选择的系统</translation>
    </message>
</context>
<context>
    <name>ConfEditDialog</name>
    <message>
        <location filename="confeditdialog.ui" line="22"/>
        <source>Config editor</source>
        <translation>配置编辑器</translation>
    </message>
    <message>
        <location filename="confeditdialog.cpp" line="82"/>
        <source>Error</source>
        <translation>错误</translation>
    </message>
    <message>
        <location filename="confeditdialog.cpp" line="83"/>
        <source>Error mounting boot partition</source>
        <translation>挂载启动分区出错</translation>
    </message>
</context>
<context>
    <name>InitDriveThread</name>
    <message>
        <location filename="initdrivethread.cpp" line="37"/>
        <location filename="initdrivethread.cpp" line="328"/>
        <source>Mounting FAT partition</source>
        <translation>挂载 FAT 分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="60"/>
        <source>Formatting settings partition</source>
        <translation>格式化设置分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="63"/>
        <source>Error formatting settings partition</source>
        <translation>格式化设置分区时出错</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="133"/>
        <source>Saving boot files to memory</source>
        <translation>保存启动文件到存储卡</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="136"/>
        <source>Error saving boot files to memory. SD card may be damaged.</source>
        <translation type="unfinished">保存启动文件到存储卡时出错。SD 卡可能已损坏。</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="141"/>
        <location filename="initdrivethread.cpp" line="181"/>
        <source>Error unmounting system partition.</source>
        <translation type="unfinished">取消挂载系统分区时出错。</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="145"/>
        <location filename="initdrivethread.cpp" line="198"/>
        <source>Zeroing partition table</source>
        <translation>清零分区表</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="148"/>
        <location filename="initdrivethread.cpp" line="201"/>
        <source>Error zero&apos;ing MBR/GPT. SD card may be broken or advertising wrong capacity.</source>
        <translation type="unfinished">清零分区表（MBR/GPT）时出错 。SD 卡可能损坏或者广告的错误的容量。</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="152"/>
        <source>Creating partitions</source>
        <translation>创建分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="156"/>
        <source>Error partitioning</source>
        <translation>分区出错</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="160"/>
        <source>Formatting boot partition (fat)</source>
        <translation>格式化启动分区(fat)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="163"/>
        <source>Error formatting boot partition (fat)</source>
        <translation>格式化启动分区时出错(fat)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="167"/>
        <source>Copying boot files to storage</source>
        <translation>复制启动文件到存储</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="192"/>
        <source>Would you like NOOBS to create one for you?
WARNING: This will erase all data on your SD card</source>
        <translation>你需要帮你创建一个吗？
警告：这会擦除你 SD 卡上所有数据</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="193"/>
        <source>Error: No MBR present on SD Card</source>
        <translation>错误：在 SD 卡上没有找到 MBR</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="206"/>
        <source>Writing new MBR</source>
        <translation>写入新的 MBR</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="214"/>
        <source>Error creating MBR</source>
        <translation>创建新的 MBR 时出错</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="222"/>
        <source>SD card has now been formatted ready for NOOBS installation. Please re-copy the NOOBS files onto the card and reboot</source>
        <translation type="unfinished">SD 卡已经被格式化，可以安装 NOOBS了。请重新复制 NOOBS 文件到 SD 卡上，并且重新启动</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="227"/>
        <source>SD card has not been formatted correctly. Please reformat using the SD Association Formatting Tool and try again.</source>
        <translation type="unfinished">SD 卡未能正确格式化。请使用 SD 协会格式化工具格式化然后再试。</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="233"/>
        <source>Removing partitions 2,3,4</source>
        <translation>移除 2,3,4 分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="247"/>
        <source>Resizing FAT partition</source>
        <translation>改变 FAT 分区大小</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="266"/>
        <source>Error resizing existing FAT partition</source>
        <translation>改变已有 FAT 分区大小时出错</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="272"/>
        <source>Creating extended partition</source>
        <translation>创建扩展分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="311"/>
        <source>Error creating extended partition</source>
        <translation>创建扩展分区时出错</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="331"/>
        <source>Error mounting system partition.</source>
        <translation type="unfinished">挂载系统分区时出错</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="67"/>
        <source>Editing cmdline.txt</source>
        <translation>编辑 cmdline.txt</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="77"/>
        <source>Error opening %1</source>
        <translation>打开 %1 失败</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="90"/>
        <source>Writing RiscOS blob</source>
        <translation>写入 RiscOS blob</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="93"/>
        <source>Error writing RiscOS blob</source>
        <translation>写入 RiscOS blob 时出错</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="100"/>
        <source>Unmounting boot partition</source>
        <translation>取消挂载启动分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="103"/>
        <source>Finish writing to disk (sync)</source>
        <translation>写入磁盘完成 (sync)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="113"/>
        <source>Mounting boot partition again</source>
        <translation>再次挂载启动分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="124"/>
        <source>SD card broken (writes do not persist)</source>
        <translation type="unfinished">SD 卡损坏 (writes do not persist)</translation>
    </message>
</context>
<context>
    <name>LanguageDialog</name>
    <message>
        <location filename="languagedialog.ui" line="135"/>
        <source>Language (l): </source>
        <translation>选择语言 (l)：</translation>
    </message>
    <message>
        <location filename="languagedialog.ui" line="156"/>
        <source>Keyboard</source>
        <translation>键盘布局</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.ui" line="47"/>
        <source>Disk space</source>
        <translation>磁盘空间</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="53"/>
        <source>Needed:</source>
        <translation>需要：</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="60"/>
        <source>Available:</source>
        <translation>可用：</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="122"/>
        <source>Install (i)</source>
        <translation>安装 (i)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="125"/>
        <source>Installs the selected operating system image onto this SD card.</source>
        <translation>安装选中的操作系统到镜像到这张 SD 卡</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="128"/>
        <source>I</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="137"/>
        <source>Exit (Esc)</source>
        <translation>退出 (Esc)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="140"/>
        <source>Exits and boots the installed operating system.</source>
        <translation>退出并启动到已安装的操作系统。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="155"/>
        <source>Advanced</source>
        <translation>高级</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="167"/>
        <source>Edit config (e)</source>
        <translation>编辑配置 (e)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="170"/>
        <source>Opens the config.txt and cmdline.txt for the installed OS in a text editor.</source>
        <translation>在文本编辑器中打开已安装的操作系统的 config.txt 和 cmdline.txt。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="182"/>
        <source>Online help (h)</source>
        <translation>在线帮助 (h)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="185"/>
        <source>Opens a web browser displaying the Raspberry Pi Forum.</source>
        <translation>打开网页浏览器访问 Raspberry Pi 论坛</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="251"/>
        <location filename="mainwindow.cpp" line="526"/>
        <location filename="mainwindow.cpp" line="942"/>
        <source>RECOMMENDED</source>
        <translation>推荐</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="406"/>
        <source>Confirm</source>
        <translation>确认</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="501"/>
        <source>NOOBS v%1 - Built: %2</source>
        <translation type="unfinished">NOOBS v%1 - Built: %2</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="85"/>
        <source>Setting up SD card</source>
        <translation>设置 SD 卡</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="149"/>
        <source>Error mounting settings partition</source>
        <translation>挂截设置分区时出错</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="150"/>
        <source>Persistent settings partition seems corrupt. Reformat?</source>
        <translation>持久设置分区好像错乱了。重新格式化吗？</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="157"/>
        <source>Reformat failed</source>
        <translation>重新格式化失败</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="157"/>
        <source>SD card might be damaged</source>
        <translation>SD 卡可能已经损坏</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="407"/>
        <source>Warning: this will install the selected Operating System(s). All existing data on the SD card will be overwritten, including any OSes that are already installed.</source>
        <translation>警告：这会安装选中的操作系统。SD 卡上包括已经安装的操作系统在内的所有的已有数据都会被覆盖。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="480"/>
        <location filename="mainwindow.cpp" line="906"/>
        <source>Error</source>
        <translation>错误</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="254"/>
        <location filename="mainwindow.cpp" line="529"/>
        <source>INSTALLED</source>
        <translation>已安装</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="626"/>
        <source>Display Mode Changed</source>
        <translation>显示模式已改变</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="558"/>
        <source>HDMI preferred mode</source>
        <translation>HDMI 首选模式</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="136"/>
        <source>Please wait while NOOBS initialises</source>
        <translation>NOOBS 初始化请等待</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="447"/>
        <source>The install process will begin shortly.</source>
        <translation>安装过程将会很快开始。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="470"/>
        <source>OS(es) installed</source>
        <translation>已安装操作系统</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="471"/>
        <source>OS(es) Installed Successfully</source>
        <translation>操作系统已安装成功</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="562"/>
        <source>HDMI safe mode</source>
        <translation>HDMI 安全模式</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="566"/>
        <source>composite PAL mode</source>
        <translation>复合 PAL 模式</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="570"/>
        <source>composite NTSC mode</source>
        <translation>复合 NTSC 模式</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="627"/>
        <source>Display mode changed to %1
Would you like to make this setting permanent?</source>
        <translation>显示模式已改变为 %1
需要要保存这一设置？</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="892"/>
        <location filename="mainwindow.cpp" line="1255"/>
        <location filename="mainwindow.cpp" line="1265"/>
        <source>Download error</source>
        <translation>出错</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="892"/>
        <source>Error downloading distribution list from Internet</source>
        <translation>下载出错</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="906"/>
        <source>Error parsing list.json downloaded from server</source>
        <translation>解析已下载的 list.json 出错</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1145"/>
        <source>Needed</source>
        <translation>需要</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1146"/>
        <source>Available</source>
        <translation>可用</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1255"/>
        <source>Error downloading meta file</source>
        <translation>下载元数据文件出错</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1265"/>
        <source>Error writing downloaded file to SD card. SD card or file system may be damaged.</source>
        <translation>写入已下载的文件到 SD 卡时出错。SD 卡或者文件系统可能已损坏。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1380"/>
        <source>Wired network access is required to use NOOBS without local images. Please insert a network cable into the network port.</source>
        <translation>没有本地镜像时使用 NOOBS 必需要使用有线网络连接，请插入网线。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="747"/>
        <location filename="mainwindow.cpp" line="1379"/>
        <source>No network access</source>
        <translation>没有网络链接</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="748"/>
        <source>Wired network access is required for this feature. Please insert a network cable into the network port.</source>
        <translation>这一功能必需使用有线网络连接，请插入网线。</translation>
    </message>
</context>
<context>
    <name>MultiImageWriteThread</name>
    <message>
        <location filename="multiimagewritethread.cpp" line="44"/>
        <source>partitions.json invalid</source>
        <translation>无效的 partitions.json</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="69"/>
        <source>RISCOS cannot be installed. Size of recovery partition too large.</source>
        <translation>不能安装 RISCOS，恢复分区太大。</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="93"/>
        <source>Not enough disk space. Need %1 MB, got %2 MB</source>
        <translation>没有足够的磁盘空间，需要 %1 MB，只剩下 %2 MB</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="97"/>
        <source>Clearing existing EBR</source>
        <translation>清除已有 EBR</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="123"/>
        <source>Finish writing (sync)</source>
        <translation>写入完成 (sync)</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="164"/>
        <source>File &apos;%1&apos; does not exist</source>
        <translation type="unfinished">文件 &apos;%1&apos; 不存在</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="187"/>
        <source>Nominal partition size not specified or zero</source>
        <translation>最小分区大小为零或者没有指定</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="213"/>
        <source>%1: Creating partition entry</source>
        <translation>%1：创建分区项</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="219"/>
        <source>%1: Writing OS image</source>
        <translation>%1：写入操作系统镜像</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="233"/>
        <source>%1: Mounting file system</source>
        <translation>%1：挂载文件系统</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="236"/>
        <location filename="multiimagewritethread.cpp" line="280"/>
        <source>%1: Error mounting file system</source>
        <translation>%1：挂载文件系统时出错</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="226"/>
        <source>%1: Creating filesystem (%2)</source>
        <translation>%1：创建文件系统 (%2)</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="140"/>
        <source>No partitions defined in partitions.json</source>
        <translation type="unfinished">partitions.json 文件中未定义分区</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="241"/>
        <source>%1: Downloading and extracting filesystem</source>
        <translation>%1：下载并解压文件系统</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="243"/>
        <source>%1: Extracting filesystem</source>
        <translation>%1：解压文件系统</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="277"/>
        <source>%1: Mounting FAT partition</source>
        <translation>%1：挂载 FAT 分区</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="284"/>
        <source>%1: Creating os_config.json</source>
        <translation>%1：创建 os_config.json</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="298"/>
        <source>%1: Saving display mode to config.txt</source>
        <translation>%1：保存显示模式到 config.txt</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="309"/>
        <source>%1: Running partition setup script</source>
        <translation>%1：运行分区安装脚本</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="352"/>
        <source>%1: Error executing partition setup script</source>
        <translation>%1：运行分区安装脚本时出错</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="357"/>
        <source>%1: Unmounting FAT partition</source>
        <translation>%1：取消挂载 FAT 分区</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="360"/>
        <source>%1: Error unmounting</source>
        <translation>%1：取消挂载时出错</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="415"/>
        <source>Error opening /dev/mmcblk0 for writing</source>
        <translation>打开写入 /dev/mmcb1k0 文件时出错</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="433"/>
        <source>Internal error in partitioning</source>
        <translation>分区时内部错误</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="516"/>
        <source>Error creating file system</source>
        <translation>创建文件系统时出错</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="558"/>
        <location filename="multiimagewritethread.cpp" line="620"/>
        <source>Unknown compression format file extension. Expecting .lzo, .gz, .xz, .bz2 or .zip</source>
        <translation>未知的压缩文件格式，期望的格式为 .lzo， .gz， .xz， .bz2 及 .zip </translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="582"/>
        <source>Error downloading or extracting tarball</source>
        <translation>下载或解压时出错</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="641"/>
        <source>Error downloading or writing OS to SD card</source>
        <translation>下载或写入SD 卡时出错</translation>
    </message>
</context>
<context>
    <name>ProgressSlideshowDialog</name>
    <message>
        <location filename="progressslideshowdialog.cpp" line="138"/>
        <source>%1 MB of %2 MB written (%3 MB/sec)</source>
        <translation>已写入 %1 MB / %2 MB (%3 MB/s)</translation>
    </message>
    <message>
        <location filename="progressslideshowdialog.cpp" line="143"/>
        <source>%1 MB written (%2 MB/sec)</source>
        <translation>已写入 %1 MB (%2 MB/s)</translation>
    </message>
</context>
<context>
    <name>QDialogButtonBox</name>
    <message>
        <location filename="languagedialog.cpp" line="27"/>
        <source>OK</source>
        <translation>确定</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="29"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="30"/>
        <source>&amp;Cancel</source>
        <translation type="unfinished">取消(&amp;C)</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="31"/>
        <source>Close</source>
        <translation>关闭</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="32"/>
        <source>&amp;Close</source>
        <translation type="unfinished">关闭(&amp;C)</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="33"/>
        <source>&amp;Yes</source>
        <translation type="unfinished">是(&amp;Y)</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="34"/>
        <source>&amp;No</source>
        <translation type="unfinished">否(&amp;N)</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="28"/>
        <source>&amp;OK</source>
        <translation type="unfinished">确定(&amp;O)</translation>
    </message>
</context>
</TS>
