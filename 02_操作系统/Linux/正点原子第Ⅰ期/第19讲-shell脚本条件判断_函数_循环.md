# shell脚本条件判断-函数-循环

## 一、shell脚本条件判断

> ### 1、if条件判断
>
> 例程1：
>
> ```shell
> #!/bin/bash
> read -p "please input(Y/N):" value
> if [ "$value" == "Y" ] || [ "$value" == "y" ]; then
> 	echo "your input is Y"
> 	exit 0
> fi
> if [ "$value" == "N" ] || [ "$value" == "n" ]; then
> 	echo "your input is N"
> 	exit 0
> fi
> ```
>
> > **注意方括号内的空格：**
> >
> > ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326193240.png)
>
>  
>
> if语法格式如下：
>
> ```shell
> if condition1;
> then
>     command1
> elif condition2 
> then 
>     command2
> else
>     commandN
> fi
> ```
>
> 将例程1修改成例程2
>
> ```shell
> #!/bin/bash
> read -p "please input(Y/N):" value
> if [ "$value" == "Y" ] || [ "$value" == "y" ]; then
> 	echo "your input is Y"
> 	exit 0 
> else 
> 	echo "your input is $value"
> 	exit 0
> fi
> ```
>
>  修改成例程3：
>
> ```shell
> #!/bin/bash
> read -p "please input(Y/N):" value
> if [ "$value" == "Y" ] || [ "$value" == "y" ]; then
> 	echo "your input is Y"
> 	exit 0
> elif [ "$value" == "N" ] || [ "$value" == "n" ]; then
> 	echo "your input is N"
> 	exit 0
> else
> 	echo "your input is err"
> fi
> ```
>
> > shell语言中没有else if，是elif
>
> ### 2、case ... esac
>
> **case ... esac** 为多选择语句，与其他语言中的 switch ... case 语句类似，是一种多分枝选择结构，每个 case 分支用右圆括号开始，用两个分号 **;;** 表示 break，即执行结束，跳出整个 case ... esac 语句，esac（就是 case 反过来）作为结束标记。
>
> 可以用 case 语句匹配一个值与一个模式，如果匹配成功，执行相匹配的命令。
>
> **case ... esac** 语法格式如下：
>
> ```shell
> case $值 in
> 模式1)
>     command1
>     command2
>     ...
>     commandN
>     ;;
> 模式2)
>     command1
>     command2
>     ...
>     commandN
>     ;;
> esac
> ```
>
> 例程：
>
> ```shell
> #!/bin/bash
> case $1 in
> 	"a")
> 		echo "param is: a"
> 		;;
> 	"b")
> 		echo "param is: b"
> 		;;
> 	*)
> 		echo "param error"
> 		;;
> esac
> ```
>
> > 注意：① 通配符 \* 不能用双引号括起来，否则表示脚本的参数为 \* 时执行；
> >
> > ​			② $1表示脚本的第一个参数

