# 引用

[原文路径-C++ 引用详解-CSDN](https://blog.csdn.net/weixin_56935264/article/details/124669712?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522168436971516800211585526%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=168436971516800211585526&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-124669712-null-null.142^v87^control_2,239^v2^insert_chatgpt&utm_term=C%2B%2B%E5%BC%95%E7%94%A8&spm=1018.2226.3001.4187)



## 一、引入

在生活中，我们可能也会给一些同学起外号，以“张磊”同学为例，我们可以叫他“张三石”，当我们叫到这个外号的时候就会自然而然的想到“张磊”同学，”张三石”就是张磊的别名，而引用也可以这样简单理解：**在语法层面上，引用就是取别名**。

## 二、C++中较为麻烦的运算符

C++中的` *` 和 `& `有多重含义，在不同的使用条件下有不同的意思。

### 1、*

```c++
int a,b;
int *p = &a;   
a = a * b;     
```



### 2、&

```c++
int a,b;
int c = a&b;
int *p = &a;
int  a = 100;
int & ar = a;
```

