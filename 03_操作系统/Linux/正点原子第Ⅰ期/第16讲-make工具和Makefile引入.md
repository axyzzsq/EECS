# make工具和Makefile引入

## 一、make/Makefile

- 当源码文件比较多的时候就不适合通过直接输入gcc命令来编译，需要进入自动化编译工具。
- make：一般来说GNU make，是一个软件，用于将源码编译为可以执行的二进制文件，make工具主要用于完成自动化编译。make工具编译的时候需要Makefile文件提供编译文件。
- Makefile：make工具所使用的文件，Makefile指明了编译的规则。

> - 如果修改了某几个源文件，则只重新编译这几个源文件；
> - 如果某个头文件被修改了，则重新编译所有包含该头文件的源文件；

- Makefile规则

  ```
  目标:依赖
  	命令1
  	命令2
  ```

  



