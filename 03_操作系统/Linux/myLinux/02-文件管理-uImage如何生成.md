# uImage的生成

在嵌入式Linux，升级设备设备端的内核是采用导入新的uImage的方式完成。

在Linux中，编译后生成的镜像文件可以是多种格式，如vmlinux、vmlinuz、Image等。其中，vmlinux是未被压缩的镜像文件，非常大，不能直接下载到NAND中，通常放在PC机上，这也是最原始的Linux镜像文件。vmlinuz是经过压缩的内核镜像文件，可以直接下载到NAND中。Image是经过objcopy工具制作的烧录镜像文件，可以直接烧录到NAND中。¹²³

如果你想生成Image文件，可以参考以下步骤：
1. 新建一个文件夹，例如mkdir Image。

2. 把编译boot Loader后生成的MLO、***.img文件（两个都是u-boot文件）；编译Kernel后生成的***.dtb文件（设备树文件）、zImage文件（Kernel镜像）；File System使用ubinize命令操作后生成的ubi.img文件（文件系统镜像）；这5个文件复制进Image文件夹。

3. 进入Image文件夹，执行以下命令：。

   ```shell
   mkimage -A arm -O linux -T kernel -C none -a 0x80008000 -e 0x80008000 -n "Linux" -d zImage uImage
   ```

   

   - `-A`：指定CPU架构，这里是arm。
   - `-O`：指定操作系统类型，这里是linux。
   - `-T`：指定镜像类型，这里是kernel。
   - `-C`：指定压缩类型，这里是none。
   - `-a`：指定内核的加载地址，这里是0x80008000。
   - `-e`：指定内核的执行地址，这里是0x80008000。
   - `-n`：指定内核的名称，这里是"Linux"。
   - `-d`：指定内核的源文件，这里是zImage。
   - `uImage`：指定输出文件名，这里是uImage。

 这个命令会生成uImage文件，即为Image文件。¹

[(1) Linux 系统的编译、镜像制作、以及烧录_linux源码交叉编译制作镜像文件_²º²²এ松的博客-CSDN博客.]( https://bing.com/search?q=Linux+%e7%bc%96%e8%af%91%e5%90%8e%e7%94%9f%e6%88%90%e9%95%9c%e5%83%8f%e6%96%87%e4%bb%b6)

[(2) linux内核Image/zImage/uImage的编译过程（十四）BSP-路人甲的博客-CSDN博客.](https://blog.csdn.net/m0_47799526/article/details/106203517_)

[(3) Linux内核镜像文件格式与生成过程（转） - lemaden - 博客园. ](https://www.cnblogs.com/lemaden/p/10438499.html )

[(4) Linux 系统的编译、镜像制作、以及烧录_linux源码交叉编译制作镜像文件_²º²²এ松的博客-CSDN博客.]( https://blog.csdn.net/L1643319918/article/details/124466254) 

[(5) 如何制作可引导的定制的Linux系统iso文件 - 知乎.]( https://zhuanlan.zhihu.com/p/47926853 )

- 内核加载地址&内核执行地址

  内核的加载地址和内核的执行地址是不同的，但是它们之间的差异很小。加载地址是指内核镜像整体要放置的内存空间位置，而入口地址是从内核镜像中开始执行的地址。内核的加载地址和入口地址在uImage的开头部分有定义，这是编译内核镜像时指定的。¹²³

  内核的加载地址和入口地址可以不同，但是它们之间的差异很小。如果它们不同，那么内核镜像本身要加载到内存地址为加载地址的地方，然后从入口地址开始执行内核代码。²³

  [(1) load address、entry point、 bootm address以及kernel运行地址.]( https://www.cnblogs.com/cyyljw/p/11008332.html )
  [(2) linux内核镜像的加载地址和入口地址_地址入口_爱就是恒久忍耐的博客-CSDN博客.]( https://blog.csdn.net/whahu1989/article/details/85255538 )
  [(3) linux内核镜像的加载地址和入口地址 - oude_yang - 博客园.]( https://www.cnblogs.com/Oude/p/12217750.html )