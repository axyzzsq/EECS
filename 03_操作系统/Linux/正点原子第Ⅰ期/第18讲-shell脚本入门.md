# shell脚本

> ## 一、交互式shell脚本
>
> 一个交互式脚本
>
> ```shell
> #!/bin/bash
> echo "please input your name"
> read name
> echo "your name is " $name
> ```
>
> > shell脚本-read用法
> >
> > ```shell
> > #!/bin/bash
> > read -p "please input age and height:" age height
> > echo "your age=$age,your height=$height"
> > # 注意read这一行，变量和前面的双引号要间隔至少一个空格
> > ```
>
>  ## 二、shell脚本的数值计算
>
> shell仅支持整形，数值计算使用 $((表达式))
>
> ```shell
> #!/bin/bash
> echo "please input two int num:"
> read -p "first num:" num1
> read -p "second num:" num2
> total=$(($num1+$num2))  #注意这一行表达式的等号左右都不能有空格
> echo "$num1+$num2 = $total"
> ```
>
> ## 三、test命令
>
> - test命令用于查看文件是否存在，权限等信息，可以进行数值、字符、文件等三方面的测试；
>
> - &&和||命令
>
>   cmd1 && cmd2，当cmd1执行完毕且执行正确，那么cmd2开始执行，如果cmd1执行错误，那么cmd2不执行
>
> - cmd1||cmd2当cmd1执行错误，cmd2开始执行，反置cmd2不执行。||表示它的两侧只有一个命令可以正确执行。
>
> ```shell
> #!/bin/bash
> echo "intput filename:"
> read -p "filename:" filename
> test -e $filename && echo "$filename 存在" || echo "$filename 不存在"
> ```
>
> 用test查看字符串是不是相等
>
> ```shell
> #!/bin/bash
> echo "input two string"
> read -p "firststr:" firststr
> read -p "secondstr:" secondstr
> test $firststr == $secondstr && echo "$firststr = $secondstr" || echo "$firststr != $secondstr"
> ```
>
> ## 四、中括号[]判断符
>
> ```shell
> #!/bin/bash
> echo "input two string"
> read -p "firststr:" firststr
> read -p "secondstr:" secondstr
> [ "$firststr" == "$secondstr" ] && echo "$firststr = $secondstr" || echo "$firststr != $secondstr"
> ```
>
> ```shell
> #!/bin/bash
> echo "input two string"
> read -p "firststr:" firststr
> read -p "secondstr:" secondstr
> [ "$firststr" == "siqing" ] && echo "$firststr = siqing" || echo "$firststr != siqing"
> ```
>
> 需要注意的是：中括号判断，两端的括号对内测都需要一个空格
>
> ## 五、默认变量
>
> $0~$n,表示shell脚本参数，包括shell脚本命令本身，shell脚本命令本身为$0
>
> $#:#表示最后一个参数的标号
>
> $@: 表$1,$2,$3…
>
> ```shell
> #!/bin/bash
> echo "filename:" $0
> echo "total param num" $#
> echo "whole param:" $@
> echo "first param:" $1
> echo "second param:" $2
> ```
>
> ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326193213.png)

