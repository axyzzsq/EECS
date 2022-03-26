# menuconfig过程
当我们在执行make menuconfig这个命令时，系统到底帮我们做了哪些工作呢？
这里面一共涉及到了一下几个文件我们来一一讲解

```
Linux内核根目录下的scripts文件夹
arch/$ARCH/Kconfig文件、各层目录下的Kconfig文件
Linux内核根目录下的makefile文件、各层目录下的makefile文件
Linux内核根目录下的的.config文件、arm/$ARCH/下的config文件
Linux内核根目录下的 include/generated/autoconf.h文件
```

- scripts文件夹存放的是跟make menuconfig配置界面的图形绘制相关的文件，我们作为使用者无需关心这个文件夹的内容

- 当我们执行make menuconfig命令出现上述蓝色配置界面以前，系统帮我们做了以下工作：
    - 首先系统会读取arch/$ARCH/目录下的Kconfig文件生成整个配置界面选项（Kconfig是整个linux配置机制的核心），那么ARCH环境变量的值等于多少呢？ 
    
      它是由linux内核根目录下的makefile文件决定的，在makefile下有此环境变量的定义：
      ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326192332.png)
      或者通过 make ARCH=arm menuconfig命令来生成配置界面，默认生成的界面是所有参数都是没有值的
    
      比如教务处进行考试，考试科数可能有外语、语文、数学等科，这里相当于我们选择了arm科可进行考试，系统就会读取arm/arm/kconfig文件生成配置选项（选择了arm科的卷子），系统还提供了x86科、milps科等10几门功课的考试题
    
- 假设教务处比较“仁慈”，为了怕某些同学做不错试题，还给我们准备了一份参考答案（默认配置选项），存放在arch/$ARCH/configs下，对于arm科来说就是arch/arm/configs文件夹：

    ​	![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326192347.png)

    此文件夹中有许多选项，系统会读取哪个呢？内核默认会读取linux内核根目录下.config文件作为内核的默认选项（试题的参考答案），我们一般会根据开发板的类型从中选取一个与我们开发板最接近的系列到Linux内核根目录下（选择一个最接近的参考答案）

    #cp arch/arm/configs/s3c2410_defconfig .config

- .config

    假设教务处留了一个心眼，他提供的参考答案并不完全正确（.config文件与我们的板子并不是完全匹配），这时我们可以选择直接修改.config文件然后执行make menuconfig命令读取新的选项

    但是一般我们不采取这个方案，我们选择在配置界面中通过空格、esc、回车选择某些选项选中或者不选中，最后保存退出的时候，Linux内核会把新的选项（正确的参考答案）更新到.config中，此时我们可以把.config重命名为其它文件保存起来（当你执行make distclean时系统会把.config文件删除），以后我们再配置内核时就不需要再去arch/arm/configs下考取相应的文件了，省去了重新配置的麻烦，直接将保存的.config文件复制为.config即可.

- 经过以上两步，我们可以正确的读取、配置我们需要的界面了

    那么他们如何跟makefile文件建立编译关系呢？

    当你保存make menuconfig选项时，系统会除了会自动更新.config外，还会将所有的选项以宏的形式保存在

    Linux内核根目录下的 include/generated/autoconf.h文件下

     ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326192347.png)

    内核中的源代码就都会包含以上.h文件，跟宏的定义情况进行条件编译。

    当我们需要对一个文件整体选择如是否编译时，还需要修改对应的makefile文件，例如：

     ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326192534.png)

    我们选择是否要编译s3c2410_ts.c这个文件时，makefile会根据CONFIG_TOUCHSCREEN_S3C2410来决定是编译此文件，此宏是在Kconfig文件中定义，当我们配置完成后，会出现在.config及autconf中，至此，我们就完成了整个linux内核的编译过程。

    最后我们会发现，整个linux内核配置过程中，留给用户的接口其实只有各层Kconfig、makefile文件以及对应的源文件。

    比如我们如果想要给内核增加一个功能，并且通过make menuconfig控制其声称过程。

    首先需要做的工作是：修改对应目录下的Kconfig文件，按照Kconfig语法增加对应的选项；

    其次执行make menuconfig选择编译进内核或者不编译进内核，或者编译为模块，.config文件和autoconf.h文件会自动生成；

    最后修改对应目录下的makefile文件完成编译选项的添加；

    最后的最后执行make zImage命令进行编译。
