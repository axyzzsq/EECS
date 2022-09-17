# const关键字

[C语言中const关键字的用法](https://blog.csdn.net/xingjiarong/article/details/47282255?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166333657816782412562614%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=166333657816782412562614&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~hot_rank-2-47282255-null-null.142^v47^body_digest,201^v3^add_ask&utm_term=C%E8%AF%AD%E8%A8%80%20const%E5%85%B3%E9%94%AE%E5%AD%97&spm=1018.2226.3001.4187)

关键字const用来定义常量，如果一个变量被const修饰，那么它的值就不能再被改变，我想一定有人有这样的疑问，C语言中不是有#define吗，干嘛还要用const呢，我想事物的存在一定有它自己的道理，所以说const的存在一定有它的合理性，与预编译指令相比，const修饰符有以下的优点：

1、预编译指令只是对值进行简单的替换，不能进行类型检查

2、可以保护被修饰的东西，防止意外修改，增强程序的健壮性

3、编译器通常不为普通const常量分配存储空间，而是将它们保存在符号表中，这使得它成为一个编译期间的常量，没有了存储与读内存的操作，使得它的效率也很高。



