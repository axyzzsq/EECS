# Hello World

```C
#include <stdio.h>

/* 执行命令: ./hello weidongshan 
 * argc = 2
 * argv[0] = ./hello
 * argv[1] = weidongshan
 */

int main(int argc, char **argv)
{
	if (argc >= 2)
		printf("Hello, %s!\n", argv[1]);
	else
		printf("Hello, world!\n");
	return 0;
}

```

- 编译命令

  ```shell
  gcc -o hello hello.c
  ./hello
  # 当需要执行在开发板上时需要使用交叉编译工具链
  arm-buildroot-linux-gnueabihf-gcc -o hello hello.c
  ```

- 怎么确定交叉编译器中头文件的默认路径？

  - 进入交叉编译器的目录里，执行： `find -name “stdio.h”`，它位于一个
    `“ include”`目录下的根目录里。这个`“ include”`目录，就是要找的路径。

- 怎么自己指定头文件目录？

  - 编译时，加上` -I <头文件目录>`这样的选项。

- 怎么确定交叉编译器中库文件的默认路径？

  - 进入交叉编译器的目录里，执行： `find -name lib`，可以得到` xxxx/lib`、
    `xxxx/usr/lib`，一般来说这 2 个目录就是要找的路径。如果有很多类似的 lib，
    进去看看，有很多 `.so` 文件的目录一般就是要找的路径。

- 怎么自己指定库文件目录、指定要用的库文件？

  - 编译时，加上` -L <库文件目录>`这样的选项，用来指定库目录；
    编译时，加上`-labc `这样的选项，用来指定库文件` libabc.so`。  

