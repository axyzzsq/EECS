# virtual析构函数

例程：

```cpp
#include<iostream>
using namespace std;
// 基类Base
class Base 
{
public:
    Base() 
    { 
        cout << "Base constructor" << endl; 
    }
    virtual ~Base() 
    { 
        cout << "Base destructor" << endl; 
    } // 虚析构函数
};

// 派生类Derived
class Derived : public Base 
{
public:
    Derived() 
    { 
        cout << "Derived constructor" << endl; 
    }
    ~Derived() 
    { 
        cout << "Derived destructor" << endl; 
    }
};

int main() {
    Base *p = new Derived(); // 基类指针指向派生类对象
    delete p; // 释放内存
    return 0;
}

/*
   这个代码演示了virtual析构函数的作用。如果没有virtual关键字，那么delete p只会调用基类的析构函数，而不会调用派生类的析构函      数，这样就会造成内存泄漏。但是由于基类的析构函数是虚拟的，所以delete p会根据虚拟表来动态调用派生类的析构函数，然后再调用基类    的析构函数，这样就可以正确地释放内存。
*/
```

执行结果：

![image-20230228205614229](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230228205614229.png)