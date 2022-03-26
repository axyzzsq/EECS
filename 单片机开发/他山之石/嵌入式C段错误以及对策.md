# 嵌入式C段错误常见原因以及对策

##  一、指针没有指向一块合法的内存

### 1、结构体成员指针未初始化

```c
struct student
{
   char *name;
   int score;
}stu,*pstu;

int main()
{
   strcpy(stu.name,"Jimy");
   stu.score = 99;
   return 0;
}  
```
