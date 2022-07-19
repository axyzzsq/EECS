# Makefile基本语法

> 除了终极目标，其他目标的顺序没有要求；
>
> 没有指定的时候，Makefile的第一个规则的目标就是终极目标

## 1、变量

> ### ①  ”=“  赋值
>
> Makefile中的变量类似于C语言中的宏定义，它的类型都是字符串类型
>
> ```makefile
> #Makefile 变量的使用
> objects = main.o input.o calcu.o
> main: $(objects)
> 	gcc -o main $(objects)
> ```
>
> 在 Makefile 要输出一串字符的话使用“echo”，就和 C 语言中的“printf”一样，“echo”前面加了个“@”符号，因为 Make 在执行的过程中会自动输出命令执行过程，在 命令前面加上“@”的话就不会输出命令执行过程。
>
> ```makefile
> name = zzk
> curname = $(name)
> name = zuozhongkai
> 
> print:
> 	@echo curname: $(curname)
> ```
>
>  ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326193025.png)
>
> 可以看到 curname 的值不是“zzk”，竟然是“zuozhongkai”，也就是变量“name” 最后一次赋值的结果，这就是赋值符“=”的神奇之处！借助另外一个变量，可以将变量的真实 值推到后面去定义。也就是变量的真实值取决于它所引用的变量的最后一次有效值。
>
> ###　② “:=” 赋值
>
> ```makefile
> name = zzk
> curname := $(name)
> name = zuozhongkai
> 
> print:
> 	@echo curname: $(curname)
> ```
>
>  ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326193040.png)
>
> 可以看到此时的 curname 是 zzk，不是 zuozhongkai 了。这是因为赋值符“:=” 不会使用后面定义的变量，只能使用前面已经定义好的，这就是“=”和“:=”两个的区别。
>
> ### ③ “?=” 赋值
>
> 范例1：
>
> ```makefile
> name = zzk
> curname ?= $(name)
> name = zuozhongkai
> 
> print:
> 	@echo curname: $(curname)
> ```
>
>  ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326193055.png)
>
> "?="是指如果前面已经赋值过了就用前面的赋值。
>
> 范例2：
>
> ```Makefile
> curname ?= zzk
> curname ?= zuozhongkai
> 
> print:
> 	@echo curname:$(curname)
> ```
>
>  ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326193128.png)
>
> ### ④ "+="赋值
>
> Makefile 中的变量是字符串，有时候我们需要给前面已经定义好的变量添加一些字符串进去，此时就要使用到符号“+=”，比如如下所示代码：
>
> ```Makefile
> objects = main.o inpiut.o
> objects += calcu.o
> ```
>
> 一开始变量 objects 的值为“main.o input.o”，后面我们给他追加了一个“calcu.o”，因此变量 objects 变成了“main.o input.o calcu.o”，这个就是变量的追加。

## 2、模式规则

> 在Makefile中，%表示通配符，对标等价于shell中的*
>
> ```Makefile
> %.o:%.c
> 	command
> ```

## 3、自动化变量

上面讲到的模式规则中，目标和依赖都是一系列文件，每一次对模式规则进行解析的时候都会是不同的目标和依赖文件，而命令只有一行，如何通过一行命令来从不同的依赖文件中生成对应的目标文件？**答案是自动化变量。**

| 自动化变量 | 描述                                                         |
| ---------- | ------------------------------------------------------------ |
| $@         | 规则中的目标集合，在模式规则中，如果由多个目标的话，用”$@“表示匹配模式中定义的目标集合。 |
| $%         | 当目标是函数库的时候表示规则中的目标成员名，如果目标不是函数库文件，那么其值为空。 |
| $<         | 依赖文件集合中的第一个文件，如果依赖文件是以模式(即“%”)定义的，那么“$<”就是符合模式的一系列的文件集合。 |
| $^         | 所有依赖文件的集合，使用空格分开，如果在依赖文件中有多个重复文件，”$^“会去除重复的依赖文件，只保留一份。 |
| $+         | 和“$^”类似，但是当依赖文件存在重复的话不会去除重复的依赖文件。 |
| $*         | 这个变量表示目标模式中“%”及其之前的部分，如果目标是test/a.test.c，目标模式为a.%,c，那么"$*"就是test/a.test。 |
| $?         | 所有比目标新的依赖目标集合，以空格分开。                     |

常用的有三种：$@、$<、$^

例程：

```Makefile
objects = main.o input.o calcu.o 
main: $(objects) 
	gcc -o main $(objects) 
%.o : %.c
	gcc -c $< 
clean:
	rm *.o
	rm main
```

## 4、伪目标

> 在Makefile有时候会通过类似make clean这种方法来执行特定的动作，而不是生成新的目标。
>
> 在上面的例程中，clean作为目标，依赖文件为空，所以每次的make clean都能达到清理文件的功能。
>
> 但是当Makefile同层文件夹下面出现一个clean文件时，make clean就失效了。
>
> 为了避免同名文件的干扰，使make clean在这种情况下能够继续生效，采用伪目标的方法
>
> ```makefile
> .PHONY:clean
> clean:
> 	rm *.o
> 	rm main
> ```

## 5、Makefile条件判断



## 6、Makefile函数

