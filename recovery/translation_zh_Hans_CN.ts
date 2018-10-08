<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="zh_CN">
<context>
    <name>BootSelectionDialog</name>
    <message>
        <location filename="bootselectiondialog.ui" line="19"/>
        <location filename="bootselectiondialog.cpp" line="278"/>
        <source>Select OS to boot</source>
        <translation>选择要启动的操作系统</translation>
    </message>
    <message>
        <location filename="bootselectiondialog.cpp" line="45"/>
        <source>Cannot display boot menu</source>
        <translation>无法显示启动菜单</translation>
    </message>
    <message>
        <location filename="bootselectiondialog.cpp" line="45"/>
        <source>Error mounting settings partition</source>
        <translation>挂载 settings 分区时发生错误</translation>
    </message>
    <message>
        <location filename="bootselectiondialog.cpp" line="219"/>
        <source>HDMI safe mode</source>
        <translation>HDMI 安全模式</translation>
    </message>
    <message>
        <location filename="bootselectiondialog.cpp" line="223"/>
        <source>composite PAL mode</source>
        <translation>复合 PAL 模式</translation>
    </message>
    <message>
        <location filename="bootselectiondialog.cpp" line="227"/>
        <source>composite NTSC mode</source>
        <translation>复合 NTSC 模式</translation>
    </message>
    <message>
        <location filename="bootselectiondialog.cpp" line="283"/>
        <source>Previously selected OS will boot in %1 seconds</source>
        <translation>已经选取的操作系统将会在 %1 秒内启动</translation>
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
        <translation>挂载 boot 分区时发生错误</translation>
    </message>
</context>
<context>
    <name>InitDriveThread</name>
    <message>
        <location filename="initdrivethread.cpp" line="39"/>
        <location filename="initdrivethread.cpp" line="70"/>
        <source>Mounting FAT partition</source>
        <translation>正在挂载 FAT 分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="63"/>
        <source>Formatting settings partition</source>
        <translation>正在格式化 settings 分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="66"/>
        <source>Error formatting settings partition</source>
        <translation>格式化 settings 分区发生错误</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="73"/>
        <source>Error mounting system partition.</source>
        <translation>挂载系统分区时发生错误。</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="78"/>
        <source>Editing cmdline.txt</source>
        <translation>正在编辑 cmdline.txt</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="88"/>
        <source>Error opening %1</source>
        <translation>打开 %1 时发生错误</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="101"/>
        <source>Writing RiscOS blob</source>
        <translation>正在写入RiscOS blob</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="104"/>
        <source>Error writing RiscOS blob</source>
        <translation>写入RiscOS blob 发生错误</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="111"/>
        <source>Unmounting boot partition</source>
        <translation>正在卸载启动分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="114"/>
        <source>Finish writing to disk (sync)</source>
        <translation>写入磁盘完成 (sync)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="124"/>
        <source>Mounting boot partition again</source>
        <translation>正在再次挂载启动分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="135"/>
        <source>SD card broken (writes do not persist)</source>
        <translation>SD 卡损坏 (写入的不会保留)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="149"/>
        <location filename="initdrivethread.cpp" line="350"/>
        <source>Error unmounting system partition.</source>
        <translation>卸载系统分区时发生错误。</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="160"/>
        <source>Would you like NOOBS to create one for you?
WARNING: This will erase all data on your SD card</source>
        <translation>您想让 NOOBS 为您创建一个吗？
警告：这将会删除您 SD 卡上的所有数据</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="161"/>
        <source>Error: No MBR present on SD Card</source>
        <translation>错误： SD 卡上没有 MBR</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="166"/>
        <location filename="initdrivethread.cpp" line="354"/>
        <source>Zeroing partition table</source>
        <translation>正在将分区表归零</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="169"/>
        <location filename="initdrivethread.cpp" line="357"/>
        <source>Error zero&apos;ing MBR/GPT. SD card may be broken or advertising wrong capacity.</source>
        <translation>将 MBR/GPT 归零时发生错误。SD 卡也许坏了或是标示的容量不正确。</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="174"/>
        <source>Writing new MBR</source>
        <translation>正在写入新的 MBR</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="182"/>
        <source>Error creating MBR</source>
        <translation>创建 MBR 时发生错误</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="190"/>
        <source>SD card has now been formatted ready for NOOBS installation. Please re-copy the NOOBS files onto the card and reboot</source>
        <translation>SD 卡已被格式化，可以用于 NOOBS 安装。请重新复制 NOOBS 文件到 SD 卡上然后重新开机</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="195"/>
        <source>SD card has not been formatted correctly. Please reformat using the SD Association Formatting Tool and try again.</source>
        <translation>SD 卡没有被正确地格式化。请使用 SD 卡相关的格式化工具重新格式化然后再试一次。</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="201"/>
        <source>Removing partitions 2,3,4</source>
        <translation>正在移除分区 2,3,4</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="215"/>
        <source>Resizing FAT partition</source>
        <translation>正在调整 FAT 分区的大小</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="239"/>
        <source>Error resizing existing FAT partition</source>
        <translation>调整 FAT 分区大小时发生错误</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="245"/>
        <source>Creating extended partition</source>
        <translation>正在创建扩展分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="276"/>
        <source>Error creating extended partition</source>
        <translation>创建扩展分区时发生错误</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="342"/>
        <source>Saving boot files to memory</source>
        <translation>正在保存启动文件到内存</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="345"/>
        <source>Error saving boot files to memory. SD card may be damaged.</source>
        <translation>保存启动文件到内存时发生错误。SD 卡可能已损坏。</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="361"/>
        <source>Creating partitions</source>
        <translation>正在创建分区</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="365"/>
        <source>Error partitioning</source>
        <translation>分区时发生错误</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="369"/>
        <source>Formatting boot partition (fat)</source>
        <translation>正在格式化启动分区 (fat)</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="372"/>
        <source>Error formatting boot partition (fat)</source>
        <translation>格式化启动分区 (fat) 时发生错误</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="376"/>
        <source>Copying boot files to storage</source>
        <translation>复制启动文件到存储空间</translation>
    </message>
    <message>
        <location filename="initdrivethread.cpp" line="452"/>
        <source>Setting disk volume ID</source>
        <translation>正在设置磁盘卷 ID</translation>
    </message>
</context>
<context>
    <name>LanguageDialog</name>
    <message>
        <location filename="languagedialog.ui" line="135"/>
        <source>Language (l): </source>
        <translation>语言 (l): </translation>
    </message>
    <message>
        <location filename="languagedialog.ui" line="156"/>
        <source>Keyboard</source>
        <translation>键盘</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.ui" line="53"/>
        <source>Disk space</source>
        <translation>磁盘空间</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="65"/>
        <source>Destination drive:</source>
        <translation>目标磁盘：</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="88"/>
        <source>Needed:</source>
        <translation>需要：</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="95"/>
        <source>Available:</source>
        <translation>可用：</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="158"/>
        <source>Install (i)</source>
        <translation>安装 (i)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="161"/>
        <source>Installs the selected operating system image onto this SD card.</source>
        <translation>安装选定的操作系统到这张 SD 卡。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="164"/>
        <source>I</source>
        <translation>I</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="173"/>
        <source>Exit (Esc)</source>
        <translation>退出 (Esc)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="176"/>
        <source>Exits and boots the installed operating system.</source>
        <translation>退出并启动已安装的操作系统。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="191"/>
        <source>Advanced</source>
        <translation>高级</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="203"/>
        <source>Edit config (e)</source>
        <translation>编辑配置 (e)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="206"/>
        <source>Opens the config.txt and cmdline.txt for the installed OS in a text editor.</source>
        <translation>在文本编辑器里打开已安装的操作系统的 config.txt 和 cmdline.txt。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="218"/>
        <source>Online help (h)</source>
        <translation>在线帮助 (h)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="221"/>
        <source>Opens a web browser displaying the Raspberry Pi Forum.</source>
        <translation>打开浏览器显示 Raspberry Pi 论坛。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="236"/>
        <source>Wifi networks (w)</source>
        <translation>Wifi 网络 (w)</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="239"/>
        <source>Select a wifi network to connect to</source>
        <translation>选择要连接的 wifi 网络</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="242"/>
        <source>W</source>
        <translation>W</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="98"/>
        <source>Setting up SD card</source>
        <translation>正在设置 SD 卡</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="119"/>
        <source>Waiting for SD card (settings partition)</source>
        <translation>正在等待 SD 卡 (settings 分区)</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="132"/>
        <source>Mounting settings partition</source>
        <translation>正在挂载 settings 分区</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="143"/>
        <source>Error mounting settings partition</source>
        <translation>挂载 settings 分区时发生错误</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="144"/>
        <source>Persistent settings partition seems corrupt. Reformat?</source>
        <translation>持久的 settings 分区似乎已损坏。重新格式化？</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="151"/>
        <source>Reformat failed</source>
        <translation>重新格式化失败</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="151"/>
        <source>SD card might be damaged</source>
        <translation>SD 卡也许可能已被损坏</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="253"/>
        <source>Please wait while NOOBS initialises</source>
        <translation>NOOBS 初始化中，请等待</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="341"/>
        <location filename="mainwindow.cpp" line="1280"/>
        <location filename="mainwindow.cpp" line="1841"/>
        <source>RECOMMENDED</source>
        <translation>建议</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="344"/>
        <source>INSTALLED</source>
        <translation>已安装</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="588"/>
        <source>Warning: this will install the selected Operating System(s). All existing data on the SD card will be overwritten, including any OSes that are already installed.</source>
        <translation>警告：这将会安装选定的操作系统。SD 卡上已有数据将被覆盖，包括任何已安装的操作系统。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="590"/>
        <source>SD card</source>
        <translation>SD 卡</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="590"/>
        <source>drive</source>
        <translation>磁盘</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="593"/>
        <location filename="mainwindow.cpp" line="612"/>
        <source>Confirm</source>
        <translation>确认</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="613"/>
        <source>Warning: incompatible Operating System(s) detected. The following OSes aren&apos;t supported on this revision of Raspberry Pi and may fail to boot or function correctly:</source>
        <translation>警告：检测到不兼容的操作系统。此版本 Raspberry Pi 不支持以下操作系统，可能会启动失败或有功能不正常：</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="655"/>
        <source>The install process will begin shortly.</source>
        <translation>安装过程将很快开始。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="677"/>
        <source>OS(es) installed</source>
        <translation>操作系统已安装</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="678"/>
        <source>OS(es) Installed Successfully</source>
        <translation>操作系统已成功安装</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="689"/>
        <location filename="mainwindow.cpp" line="1178"/>
        <source>Error</source>
        <translation>错误</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="706"/>
        <source>NOOBS v%1 - Built: %2</source>
        <translation>NOOBS v%1 - 构建时间: %2</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="738"/>
        <source>HDMI preferred mode</source>
        <translation>HDMI 首选模式</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="742"/>
        <source>HDMI safe mode</source>
        <translation>HDMI 安全模式</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="746"/>
        <source>composite PAL mode</source>
        <translation>复合 PAL 模式</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="750"/>
        <source>composite NTSC mode</source>
        <translation>复合 NTSC 模式</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="806"/>
        <source>Display Mode Changed</source>
        <translation>显示模式已改动</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="807"/>
        <source>Display mode changed to %1
Would you like to make this setting permanent?</source>
        <translation>显示模式已改为 %1
您想要让这个改动永久生效吗？</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="925"/>
        <location filename="mainwindow.cpp" line="1631"/>
        <location filename="mainwindow.cpp" line="1639"/>
        <source>No network access</source>
        <translation>无法访问网络</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="926"/>
        <source>Wired network access is required for this feature. Please insert a network cable into the network port.</source>
        <translation>这个功能需要联网。请插入网线到有线网口。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1164"/>
        <location filename="mainwindow.cpp" line="1505"/>
        <location filename="mainwindow.cpp" line="1515"/>
        <source>Download error</source>
        <translation>下载错误</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1164"/>
        <source>Error downloading distribution list from Internet</source>
        <translation>从互联网下载分发清单时发生错误</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1178"/>
        <source>Error parsing list.json downloaded from server</source>
        <translation>解析从服务器上下载的 list.jsong上发生错误</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1398"/>
        <source>Needed</source>
        <translation>需要</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1399"/>
        <source>Available</source>
        <translation>可用</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1505"/>
        <source>Error downloading meta file</source>
        <translation>下载 meta 文件时发生错误</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1515"/>
        <source>Error writing downloaded file to SD card. SD card or file system may be damaged.</source>
        <translation>将已下载的文件写入 SD 卡时发生错误。SD 卡 或文件系统可能已损坏。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1632"/>
        <source>Network access is required to use NOOBS without local images. Please select your wifi network in the next screen.</source>
        <translation>NOOBS 不使用本地映像文件时需要联网。请在下一步选择您的 wifi 网络。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1640"/>
        <source>Wired network access is required to use NOOBS without local images. Please insert a network cable into the network port.</source>
        <translation>NOOBS 不使用本地映像文件时需要联网。请在插入网线到有线接口。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1754"/>
        <source>Reformat drive?</source>
        <translation>重新格式化磁盘？</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1755"/>
        <source>Are you sure you want to reformat the drive &apos;%1&apos; for use with NOOBS? All existing data on the drive will be deleted!</source>
        <translation>您确定要重新格式化磁盘 &apos;%1&apos;供 NOOBS 使用吗？所有在该磁盘上的数据都会被删除！</translation>
    </message>
</context>
<context>
    <name>MultiImageWriteThread</name>
    <message>
        <location filename="multiimagewritethread.cpp" line="55"/>
        <source>partitions.json invalid</source>
        <translation>partitions.json 无效</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="68"/>
        <source>RISCOS cannot be installed. RISCOS offset value mismatch.</source>
        <translation>RISCOS 无法被安装。RISCOS 偏移量不符合。</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="74"/>
        <source>RISCOS cannot be installed. RISCOS offset value missing.</source>
        <translation>RISCOS 无法被安装。RISCOS 偏移量缺失。</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="79"/>
        <source>RISCOS cannot be installed. Size of recovery partition too large.</source>
        <translation>RISCOS 无法被安装。recovery 分区的大小太大。</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="106"/>
        <source>More than one operating system requires partition number %1</source>
        <translation>多个操作系统需要分区号 %1</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="111"/>
        <source>Operating system cannot require a system partition (1,5)</source>
        <translation>操作系统无法请求系统分区 (1,5)</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="116"/>
        <source>Operating system cannot claim both primary partitions 2 and 4</source>
        <translation>操作系统无法同时占有主分区 2 和 4</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="144"/>
        <source>Not enough disk space. Need %1 MB, got %2 MB</source>
        <translation>磁盘空间不足。需要 %1 MB，剩余 %2 MB</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="214"/>
        <source>Fixed partition offset too low</source>
        <translation>固定分区偏移量过低</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="275"/>
        <source>Writing partition table</source>
        <translation>正在写入分区表</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="290"/>
        <source>Writing boot partition table</source>
        <translation>正在写入启动分区表</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="280"/>
        <location filename="multiimagewritethread.cpp" line="296"/>
        <source>Zero&apos;ing start of each partition</source>
        <translation>正在将每个分区的开头归零</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="320"/>
        <source>Finish writing (sync)</source>
        <translation>写入完成 (sync)</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="340"/>
        <source>Error reading existing partition table</source>
        <translation>读取已有分区表时发生错误</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="425"/>
        <source>Error creating partition table</source>
        <translation>创建分区表时发生错误</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="457"/>
        <source>File &apos;%1&apos; does not exist</source>
        <translation>文件 &apos;%1&apos; 不存在</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="467"/>
        <location filename="multiimagewritethread.cpp" line="473"/>
        <source>%1: Writing OS image</source>
        <translation>%1：正在写入操作系统映像文件</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="480"/>
        <source>%1: Creating filesystem (%2)</source>
        <translation>%1：正在创建文件系统 (%2)</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="487"/>
        <source>%1: Mounting file system</source>
        <translation>%1：正在挂载文件系统</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="495"/>
        <location filename="multiimagewritethread.cpp" line="519"/>
        <source>%1: Error mounting file system</source>
        <translation>%1：挂载文件系统时发生错误</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="500"/>
        <source>%1: Downloading and extracting filesystem</source>
        <translation>%1：正在下载并解压缩文件系统</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="502"/>
        <source>%1: Extracting filesystem</source>
        <translation>%1：正在解压缩文件系统</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="516"/>
        <source>%1: Mounting FAT partition</source>
        <translation>%1：正在挂载 FAT 分区</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="528"/>
        <source>%1: Creating os_config.json</source>
        <translation>%1：正在创建 os_config.json </translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="559"/>
        <source>%1: Saving display mode to config.txt</source>
        <translation>%1：正在将显示模式保存到 config.txt</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="570"/>
        <source>%1: Running partition setup script</source>
        <translation>%1：正在运行分区设置脚本</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="619"/>
        <source>%1: Error executing partition setup script</source>
        <translation>%1：运行分区设置脚本时发生错误</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="624"/>
        <source>%1: Unmounting FAT partition</source>
        <translation>%1：卸载 FAT 分区</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="627"/>
        <source>%1: Error unmounting</source>
        <translation>%1：卸载错误</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="786"/>
        <source>Error creating file system</source>
        <translation>创建文件系统时发生错误</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="828"/>
        <location filename="multiimagewritethread.cpp" line="899"/>
        <location filename="multiimagewritethread.cpp" line="958"/>
        <source>Unknown compression format file extension. Expecting .lzo, .gz, .xz, .bz2 or .zip</source>
        <translation>未知的压缩文件扩展名。只接受  .lzo, .gz, .xz, .bz2 或 .zip</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="861"/>
        <source>Error downloading or extracting tarball</source>
        <translation>下载或解压缩 tarball 时发生错误</translation>
    </message>
    <message>
        <location filename="multiimagewritethread.cpp" line="920"/>
        <location filename="multiimagewritethread.cpp" line="979"/>
        <source>Error downloading or writing OS to SD card</source>
        <translation>下载或将操作系统写入 SD 卡时发生错误</translation>
    </message>
</context>
<context>
    <name>ProgressSlideshowDialog</name>
    <message>
        <location filename="progressslideshowdialog.cpp" line="153"/>
        <source>%1 MB of %2 MB written (%3 MB/sec)</source>
        <translation>%2 MB 中的 %1 MB 已经写入 (%3 MB/秒)</translation>
    </message>
    <message>
        <location filename="progressslideshowdialog.cpp" line="158"/>
        <source>%1 MB written (%2 MB/sec)</source>
        <translation>%1 MB 已写入 (%2 MB/秒)</translation>
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
        <location filename="languagedialog.cpp" line="28"/>
        <source>&amp;OK</source>
        <translation>确定(&amp;O)</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="29"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="30"/>
        <source>&amp;Cancel</source>
        <translation>取消(&amp;C)</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="31"/>
        <source>Close</source>
        <translation>关闭</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="32"/>
        <source>&amp;Close</source>
        <translation>关闭(&amp;C)</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="33"/>
        <source>&amp;Yes</source>
        <translation>是(&amp;Y)</translation>
    </message>
    <message>
        <location filename="languagedialog.cpp" line="34"/>
        <source>&amp;No</source>
        <translation>否(&amp;N)</translation>
    </message>
</context>
<context>
    <name>WifiSettingsDialog</name>
    <message>
        <location filename="wifisettingsdialog.ui" line="22"/>
        <source>Wifi network selection</source>
        <translation>无线网络选择</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.ui" line="28"/>
        <source>Wifi network</source>
        <translation>无线网络</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.ui" line="55"/>
        <source>Authentication method</source>
        <translation>认证方法</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.ui" line="64"/>
        <source>Pressing the WPS button on my wifi router</source>
        <translation>按下我的无线路由器上的 WPS 按钮</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.ui" line="71"/>
        <source>Password authentication</source>
        <translation>密码认证</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.ui" line="96"/>
        <source>Username</source>
        <translation>用户名</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.ui" line="110"/>
        <source>Password</source>
        <translation>密码</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.cpp" line="44"/>
        <source>No wifi interfaces</source>
        <translation>没有 Wifi 接口</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.cpp" line="44"/>
        <source>No wifi interfaces available</source>
        <translation>未提供 Wifi 接口</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.cpp" line="227"/>
        <source>Authenticated but still waiting for DHCP lease</source>
        <translation>验证通过，但仍在等待 DHCP 租约</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.cpp" line="234"/>
        <source>Connecting to wifi failed</source>
        <translation>连接 Wifi 失败</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.cpp" line="234"/>
        <source>Connecting to the wifi access point failed. Check your password</source>
        <translation>连接到 Wifi 接入点失败。检查您的密码</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.cpp" line="269"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.cpp" line="275"/>
        <source>Releasing old DHCP lease</source>
        <translation>正在释放旧的 DHCP 租约</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.cpp" line="290"/>
        <source>Disconnecting</source>
        <translation>正在断开连接</translation>
    </message>
    <message>
        <location filename="wifisettingsdialog.cpp" line="295"/>
        <source>Connecting</source>
        <translation>正在连接</translation>
    </message>
</context>
</TS>
