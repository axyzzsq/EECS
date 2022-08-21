# Linux测试指令

## 1、系统信息

- uname -m：显示机器的处理器架构

- uname -r：显示正在使用的内核版本

- dmidecode -q：显示硬件系统部件

- cat /proc/cpuinfo：显示 CPU info 的信息

- cat /proc/interrupts：显示中断

- cat /proc/meminfo：显示内存使用情况

- cat /proc/swaps：显示哪些 swap 被使用

- cat /proc/version：显示内核的版本

- cat /proc/net/dev：显示网络适配器及统计

- cat /proc/mounts：显示已加载的文件系统

- date：显示系统日期

- cal 2021：显示 2021 年的日历表

## 2、关机

- shutdown -h now：关闭系统

- poweroff：关机

- init 0：关闭系统

- telinit 0：关闭系统

- shutdown -h hours:minutes &：按预定时间关闭系统

- shutdown -c：取消按预定时间关闭系统

- shutdown -r now：重启

- reboot：重启

- logout：注销

## 3、文件和目录

- cd /home：进入 "/ home" 目录

- cd .. 返回上一级目录

- cd ../.. 返回上两级目录

- cd - 返回上次所在的目录

- pwd：显示工作路径

- ls：查看目录中的文件

- ls -F：查看目录中的文件

- ls -l：显示文件和目录的详细资料

- ls -a：显示隐藏文件

- ls [0-9]：显示包含数字的文件名和目录名

- tree：显示文件和目录由根目录开始的树形结构

- lstree：显示文件和目录由根目录开始的树形结构

- mkdir dir1：创建一个叫做 ‘dir1’ 的目录’

- mkdir dir1 dir2： 同时创建两个目录

- mkdir -p /tmp/dir1/dir2：创建一个目录树

- touch file1：创建 file1 文件

- rm -f file1：删除一个叫 "file1" 的文件

- rmdir dir1：删除一个叫 "dir1" 的目录

- rm -rf dir1：删除一个叫 "dir1" 的目录并同时删除其内容

- rm -rf dir1 dir2：同时删除两个目录及它们的内容

- mv dir1 new_dir：重命名/移动 一个目录

- cp file1 file2：复制一个文件

- cp dir/* . 复制一个目录下的所有文件到当前工作目录

- cp -a /tmp/dir1 . 复制一个目录到当前工作目录

- cp -a dir1 dir2：复制一个目录

- ln -s file1 lnk1：创建一个指向文件或目录的软链接

- ln file1 lnk1：创建一个指向文件或目录的物理链接

## 4、文件搜索

- find / -name file1：从 "/" 开始进入根文件系统搜索文件和目录

- find / -user user1：搜索属于用户 "user1" 的文件和目录

- find /home/user1 -name *.bin：在目录 "/ home/user1" 中搜索带有’.bin’ 结尾的文件

- find /usr/bin -type f -atime +100：搜索在过去 100 天内未被使用过的执行文件

- find /usr/bin -type f -mtime -10：搜索在 10 天内被创建或者修改过的文件

- locate *.ps：寻找以 ".ps" 结尾的文件 - 先运行 "updatedb" 命令

- whereis halt：显示一个二进制文件、源码或 man 的位置

- which halt：显示一个二进制文件或可执行文件的完整路径

## 5、磁盘空间

- df -h：显示已经挂载的分区列表

- ls -lSr |more：以尺寸大小排列文件和目录

- du -sh dir1：估算目录 "dir1" 已经使用的磁盘空间’

- du -sk * | sort -rn：以容量大小为依据依次显示文件和目录的大小

## 6、用户和群组

- groupadd group_name：创建一个新用户组

- groupdel group_name：删除一个用户组

- groupmod -n new_group_name old_group_name：重命名一个用户组

- useradd -c "Name Surname " -g admin -d /home/user1 -s /bin/bash user1：创建一个属于 "admin" 用户组的用户

- useradd user1：创建一个新用户

- userdel -r user1：删除一个用户 ( "-r" 排除主目录)

- usermod -c "User FTP" -g system -d /ftp/user1 -s /bin/nologin user1：修改用户属性

- passwd：修改口令

- passwd user1：修改一个用户的口令 (只允许 root 执行)

- pwck：检查 "/etc/passwd" 的文件格式和语法修正以及存在的用户

- grpck：检查 "/etc/passwd" 的文件格式和语法修正以及存在的群组

## 7、文件的权限

- ls -lh：显示权限

- chmod ugo+rwx directory1：设置目录的所有人(u)、群组(g)以及其他人(o)以读（r ）、写(w)和执行(x)的权限

- chmod go-rwx directory1：删除群组(g)与其他人(o)对目录的读写执行权限

- chown user1 file1：改变一个文件的所有人属性

- chown -R user1 directory1：改变一个目录的所有人属性并同时改变改目录下所有文件的属性

- chgrp group1 file1：改变文件的群组

- chown user1:group1 file1：改变一个文件的所有人和群组属性

## 8、打包和压缩文件

- gunzip file1.gz：解压一个叫做 'file1.gz’的文件

- gzip file1：压缩一个叫做 'file1’的文件

- gzip -9 file1：最大程度压缩

- tar -cvf archive.tar file1：创建一个非压缩的 tarball

- tar -cvf archive.tar file1 file2 dir1：创建一个包含了 ‘file1’, ‘file2’ 以及 'dir1’的档案文件

- tar -tf archive.tar：显示一个包中的内容

- tar -xvf archive.tar：释放一个包

- tar -xvf archive.tar -C /tmp：将压缩包释放到 /tmp 目录下

- tar -cvfj archive.tar.bz2 dir1：创建一个 bzip2 格式的压缩包

- tar -jxvf archive.tar.bz2：解压一个 bzip2 格式的压缩包

- tar -cvfz archive.tar.gz dir1：创建一个 gzip 格式的压缩包

- tar -zxvf archive.tar.gz：解压一个 gzip 格式的压缩包

- zip file1.zip file1：创建一个 zip 格式的压缩包

- zip -r file1.zip file1 file2 dir1：将几个文件和目录同时压缩成一个 zip 格式的压缩包

- unzip file1.zip：解压一个 zip 格式压缩包

## 9、查看文件内容

- cat file1：从第一个字节开始正向查看文件的内容

- tac file1：从最后一行开始反向查看一个文件的内容

- more file1：查看一个长文件的内容

- less file1：类似于 "more" 命令，但是它允许在文件中和正向操作一样的反向操作

- head -2 file1：查看一个文件的前两行

- tail -2 file1：查看一个文件的最后两行

- tail -f /var/log/messages：实时查看被添加到一个文件中的内容

## 10、查询端口占用并杀掉占用端口的进程

1)已知程序名称查找占用了端口

查进程 ID：ps -ef|grep 程序名(如 tomcat)

根据上一步结果，查占用端口：netstat -nap I grep pid(109)

2)已知占用了哪个端口，查程序名称

查进程 ID：netstat -nap Igrep 端口(8080)

根据上步结果，查应用程序名：ps -ef| grep pid

