# Ubuntu环境配置

## git status中文显示异常问题

```shell
git config --global core.quotepath false
```

- `core.quotepath`是一个Git配置项，它控制Git是否对路径中的非ASCII字符进行转义。如果设置为false，则不会对路径中的非ASCII字符进行转义，这样就可以在Git中正确地显示中文字符。

![image-20230717100519845](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230717100519845.png)