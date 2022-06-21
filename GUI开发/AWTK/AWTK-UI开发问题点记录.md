# AWTK开发问题点记录

## 1.UI修改生效问题

1. 修改`awtk/design/default/ui/uiex/`目录下的xml文件，即修改ui布局信息；

2. 在awtk/目录下执行

	```shell
	python scripts/update_res.py ui
	```

	即可对ui进行更新，把xml文件信息更新到同目录下同文件名的bin文件中；

	> **Note：**
	>
	> 在执行update_res.py脚本的时候，需要先对awtk/目录执行scons命令编译
	>
	> - 编译awtk/目录时可能会遇到的C99编译器不支持在for循环的起始值初始化
	>
	> 	处理方式：需要修改awtk源码；
	>
	> - fatal error: gtk/gtk.h: No such file or directory**报错**
	>
	> 	处理方式：在开发环境中`sudo apt-get install libgtk-3-dev`

3. 在awtk-linux-fb/目录下执行scons；

4. 在awtk-linux-fb/目录下执行 ` sh ./release.sh`；

5. 拷贝生成文件release.tar.gz到开发板上，解压后运行/release/bin/demoui文件。

