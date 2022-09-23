# C语言assert

[C语言断言assert详解](https://blog.csdn.net/m0_46577050/article/details/122033731?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166359388816782395391130%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=166359388816782395391130&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_click~default-2-122033731-null-null.142^v47^body_digest,201^v3^add_ask&utm_term=C%E8%AF%AD%E8%A8%80assert&spm=1018.2226.3001.4187)

## 尽量利用断言来提高代码的可测试性

assert的作用是现计算表达式 expression ，如果其值为假（即为0），那么它先向stderr打印一条出错信息，然后通过调用 abort 来终止程序运行。

原型：

```C
#include <assert.h>
void assert(scalar expression);
```

例程：

```C
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
int main( void )
{
    FILE *fp;
    fp = fopen( "test.txt", "w" );//以可写的方式打开一个文件，如果不存在就创建一个同名文件
    assert( fp );                           //所以这里不会出错
    fclose( fp );
    fp = fopen( "noexitfile.txt", "r" );//以只读的方式打开一个文件，如果不存在就打开文件失败
    assert( fp );                           //所以这里出错
    fclose( fp );                           //程序永远都执行不到这里来
    return 0;
}
```

 ![image-20220919213616139](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209192136187.png)

## 用法总结与注意事项

- 在函数开始处检验传入参数的合法性，如:

  ```C
  int resetBufferSize(int nNewSize){
  
  //功能:改变缓冲区大小,
  //参数:nNewSize 缓冲区新长度
  //返回值:缓冲区当前长度
  //说明:保持原信息内容不变 nNewSize<=0表示清除缓冲区
  assert(nNewSize >= 0);
  assert(nNewSize <= MAX_BUFFER_SIZE);
  
  …
  }
  ```

- 每个assert只检验一个条件,因为同时检验多个条件时,如果断言失败,无法直观的判断是哪个条件失败

  ```C
  //不好: 
  assert(nOffset>=0 && nOffset+nSize<=m_nInfomationSize);
  
  //好: 
  assert(nOffset >= 0);
  assert(nOffset+nSize <= m_nInfomationSize);
  ```

- 不能使用改变环境的语句,因为assert只在DEBUG中生效,如果这么做,会使用程序在真正运行时遇到问题
  ```C
  //错误: 
  assert(i++ < 100)
  /*
   * 这是因为如果出错，比如在执行之前i=100,那么这条语句就不会执行，那么i++这条命令就没有执行。
   */
  //正确: 
  assert(i < 100)
  i++;
  ```

  

- assert和后面的语句应空一行,以形成逻辑和视觉上的一致感
- 有的地方,assert不能代替条件过滤
  