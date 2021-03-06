# 第9讲-Ubuntu下的磁盘管理

## 一、Ubuntu磁盘文件

/dev/sd* 文件，此类文件是磁盘设备文件，并不能直接访问磁盘，必须要将磁盘挂载到某一个目录下才可以进行访问

## 二、磁盘和目录的容量查询命令

> 1、df
>
> 2、du  
>
> >①查看一级目录
> >
> > ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326192646.png)
> >
> >②查看二级目录
> >
> > ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326192646.png)
> 
> 

## 三、磁盘挂载与卸载、分区和格式化

### 1、磁盘的挂载和卸载

>①挂载 mount
>
>②卸载 umount
>
>>挂载之后中文乱码后的解决办法：
>>
>>`sudo mount  -o  iocharset=utf8  /dev/sdb1  /media/zzk/udisk`
>
>

### 2、磁盘分区

> fdisk命令
>
>  ```shell
>  cd /dev
>  sudo fdisk /dev/sdb  #按照提示进行操作
>  #如果分区操作不能进行，要先看是被占用、挂载
>  ```
>
> > 给分区1分配1GB空间
> >
> > 1GB = 1024M = 1024 * 1024KB = 1024 *  1024 * 1024B = 1073741824B
> >
> > 1个扇区512B
> >
> > 1073741824 / 512 =  2097152个扇区
> >
> > 分区的截止扇区指的是绝对值，要把前面已经被占用的扇区数目全部加上。

### 3、格式化

> ***格式化就是给每个分区装一个文件系统***，比如FAT,EXT等。
>
> 使用命令—— mkfd，如：
>
> `mkfs -t vfat /dev/sd1`
>
> 分成三个分区之后的U盘插入电脑在win系统下是作为U盘三个盘符显示出来，Linux的恢复方法是删除三个分区，然后创建一个大的分区

> windows系统下，可以使用SDFormatter快速进行格式化
